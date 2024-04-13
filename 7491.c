smb2_query_info_compound(const unsigned int xid, struct cifs_tcon *tcon,
			 const char *path, u32 desired_access,
			 u32 class, u32 type, u32 output_len,
			 struct kvec *rsp, int *buftype,
			 struct cifs_sb_info *cifs_sb)
{
	struct cifs_ses *ses = tcon->ses;
	struct TCP_Server_Info *server = cifs_pick_channel(ses);
	int flags = CIFS_CP_CREATE_CLOSE_OP;
	struct smb_rqst rqst[3];
	int resp_buftype[3];
	struct kvec rsp_iov[3];
	struct kvec open_iov[SMB2_CREATE_IOV_SIZE];
	struct kvec qi_iov[1];
	struct kvec close_iov[1];
	u8 oplock = SMB2_OPLOCK_LEVEL_NONE;
	struct cifs_open_parms oparms;
	struct cifs_fid fid;
	int rc;
	__le16 *utf16_path;
	struct cached_fid *cfid = NULL;

	if (!path)
		path = "";
	utf16_path = cifs_convert_path_to_utf16(path, cifs_sb);
	if (!utf16_path)
		return -ENOMEM;

	if (smb3_encryption_required(tcon))
		flags |= CIFS_TRANSFORM_REQ;

	memset(rqst, 0, sizeof(rqst));
	resp_buftype[0] = resp_buftype[1] = resp_buftype[2] = CIFS_NO_BUFFER;
	memset(rsp_iov, 0, sizeof(rsp_iov));

	rc = open_cached_dir(xid, tcon, path, cifs_sb, &cfid);

	memset(&open_iov, 0, sizeof(open_iov));
	rqst[0].rq_iov = open_iov;
	rqst[0].rq_nvec = SMB2_CREATE_IOV_SIZE;

	oparms.tcon = tcon;
	oparms.desired_access = desired_access;
	oparms.disposition = FILE_OPEN;
	oparms.create_options = cifs_create_options(cifs_sb, 0);
	oparms.fid = &fid;
	oparms.reconnect = false;

	rc = SMB2_open_init(tcon, server,
			    &rqst[0], &oplock, &oparms, utf16_path);
	if (rc)
		goto qic_exit;
	smb2_set_next_command(tcon, &rqst[0]);

	memset(&qi_iov, 0, sizeof(qi_iov));
	rqst[1].rq_iov = qi_iov;
	rqst[1].rq_nvec = 1;

	if (cfid) {
		rc = SMB2_query_info_init(tcon, server,
					  &rqst[1],
					  cfid->fid->persistent_fid,
					  cfid->fid->volatile_fid,
					  class, type, 0,
					  output_len, 0,
					  NULL);
	} else {
		rc = SMB2_query_info_init(tcon, server,
					  &rqst[1],
					  COMPOUND_FID,
					  COMPOUND_FID,
					  class, type, 0,
					  output_len, 0,
					  NULL);
	}
	if (rc)
		goto qic_exit;
	if (!cfid) {
		smb2_set_next_command(tcon, &rqst[1]);
		smb2_set_related(&rqst[1]);
	}

	memset(&close_iov, 0, sizeof(close_iov));
	rqst[2].rq_iov = close_iov;
	rqst[2].rq_nvec = 1;

	rc = SMB2_close_init(tcon, server,
			     &rqst[2], COMPOUND_FID, COMPOUND_FID, false);
	if (rc)
		goto qic_exit;
	smb2_set_related(&rqst[2]);

	if (cfid) {
		rc = compound_send_recv(xid, ses, server,
					flags, 1, &rqst[1],
					&resp_buftype[1], &rsp_iov[1]);
	} else {
		rc = compound_send_recv(xid, ses, server,
					flags, 3, rqst,
					resp_buftype, rsp_iov);
	}
	if (rc) {
		free_rsp_buf(resp_buftype[1], rsp_iov[1].iov_base);
		if (rc == -EREMCHG) {
			tcon->need_reconnect = true;
			pr_warn_once("server share %s deleted\n",
				     tcon->treeName);
		}
		goto qic_exit;
	}
	*rsp = rsp_iov[1];
	*buftype = resp_buftype[1];

 qic_exit:
	kfree(utf16_path);
	SMB2_open_free(&rqst[0]);
	SMB2_query_info_free(&rqst[1]);
	SMB2_close_free(&rqst[2]);
	free_rsp_buf(resp_buftype[0], rsp_iov[0].iov_base);
	free_rsp_buf(resp_buftype[2], rsp_iov[2].iov_base);
	if (cfid)
		close_cached_dir(cfid);
	return rc;
}