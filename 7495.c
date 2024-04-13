smb2_query_dir_first(const unsigned int xid, struct cifs_tcon *tcon,
		     const char *path, struct cifs_sb_info *cifs_sb,
		     struct cifs_fid *fid, __u16 search_flags,
		     struct cifs_search_info *srch_inf)
{
	__le16 *utf16_path;
	struct smb_rqst rqst[2];
	struct kvec rsp_iov[2];
	int resp_buftype[2];
	struct kvec open_iov[SMB2_CREATE_IOV_SIZE];
	struct kvec qd_iov[SMB2_QUERY_DIRECTORY_IOV_SIZE];
	int rc, flags = 0;
	u8 oplock = SMB2_OPLOCK_LEVEL_NONE;
	struct cifs_open_parms oparms;
	struct smb2_query_directory_rsp *qd_rsp = NULL;
	struct smb2_create_rsp *op_rsp = NULL;
	struct TCP_Server_Info *server = cifs_pick_channel(tcon->ses);
	int retry_count = 0;

	utf16_path = cifs_convert_path_to_utf16(path, cifs_sb);
	if (!utf16_path)
		return -ENOMEM;

	if (smb3_encryption_required(tcon))
		flags |= CIFS_TRANSFORM_REQ;

	memset(rqst, 0, sizeof(rqst));
	resp_buftype[0] = resp_buftype[1] = CIFS_NO_BUFFER;
	memset(rsp_iov, 0, sizeof(rsp_iov));

	/* Open */
	memset(&open_iov, 0, sizeof(open_iov));
	rqst[0].rq_iov = open_iov;
	rqst[0].rq_nvec = SMB2_CREATE_IOV_SIZE;

	oparms.tcon = tcon;
	oparms.desired_access = FILE_READ_ATTRIBUTES | FILE_READ_DATA;
	oparms.disposition = FILE_OPEN;
	oparms.create_options = cifs_create_options(cifs_sb, 0);
	oparms.fid = fid;
	oparms.reconnect = false;

	rc = SMB2_open_init(tcon, server,
			    &rqst[0], &oplock, &oparms, utf16_path);
	if (rc)
		goto qdf_free;
	smb2_set_next_command(tcon, &rqst[0]);

	/* Query directory */
	srch_inf->entries_in_buffer = 0;
	srch_inf->index_of_last_entry = 2;

	memset(&qd_iov, 0, sizeof(qd_iov));
	rqst[1].rq_iov = qd_iov;
	rqst[1].rq_nvec = SMB2_QUERY_DIRECTORY_IOV_SIZE;

	rc = SMB2_query_directory_init(xid, tcon, server,
				       &rqst[1],
				       COMPOUND_FID, COMPOUND_FID,
				       0, srch_inf->info_level);
	if (rc)
		goto qdf_free;

	smb2_set_related(&rqst[1]);

again:
	rc = compound_send_recv(xid, tcon->ses, server,
				flags, 2, rqst,
				resp_buftype, rsp_iov);

	if (rc == -EAGAIN && retry_count++ < 10)
		goto again;

	/* If the open failed there is nothing to do */
	op_rsp = (struct smb2_create_rsp *)rsp_iov[0].iov_base;
	if (op_rsp == NULL || op_rsp->hdr.Status != STATUS_SUCCESS) {
		cifs_dbg(FYI, "query_dir_first: open failed rc=%d\n", rc);
		goto qdf_free;
	}
	fid->persistent_fid = op_rsp->PersistentFileId;
	fid->volatile_fid = op_rsp->VolatileFileId;

	/* Anything else than ENODATA means a genuine error */
	if (rc && rc != -ENODATA) {
		SMB2_close(xid, tcon, fid->persistent_fid, fid->volatile_fid);
		cifs_dbg(FYI, "query_dir_first: query directory failed rc=%d\n", rc);
		trace_smb3_query_dir_err(xid, fid->persistent_fid,
					 tcon->tid, tcon->ses->Suid, 0, 0, rc);
		goto qdf_free;
	}

	atomic_inc(&tcon->num_remote_opens);

	qd_rsp = (struct smb2_query_directory_rsp *)rsp_iov[1].iov_base;
	if (qd_rsp->hdr.Status == STATUS_NO_MORE_FILES) {
		trace_smb3_query_dir_done(xid, fid->persistent_fid,
					  tcon->tid, tcon->ses->Suid, 0, 0);
		srch_inf->endOfSearch = true;
		rc = 0;
		goto qdf_free;
	}

	rc = smb2_parse_query_directory(tcon, &rsp_iov[1], resp_buftype[1],
					srch_inf);
	if (rc) {
		trace_smb3_query_dir_err(xid, fid->persistent_fid, tcon->tid,
			tcon->ses->Suid, 0, 0, rc);
		goto qdf_free;
	}
	resp_buftype[1] = CIFS_NO_BUFFER;

	trace_smb3_query_dir_done(xid, fid->persistent_fid, tcon->tid,
			tcon->ses->Suid, 0, srch_inf->entries_in_buffer);

 qdf_free:
	kfree(utf16_path);
	SMB2_open_free(&rqst[0]);
	SMB2_query_directory_free(&rqst[1]);
	free_rsp_buf(resp_buftype[0], rsp_iov[0].iov_base);
	free_rsp_buf(resp_buftype[1], rsp_iov[1].iov_base);
	return rc;
}