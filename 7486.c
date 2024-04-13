smb2_query_reparse_tag(const unsigned int xid, struct cifs_tcon *tcon,
		   struct cifs_sb_info *cifs_sb, const char *full_path,
		   __u32 *tag)
{
	int rc;
	__le16 *utf16_path = NULL;
	__u8 oplock = SMB2_OPLOCK_LEVEL_NONE;
	struct cifs_open_parms oparms;
	struct cifs_fid fid;
	struct TCP_Server_Info *server = cifs_pick_channel(tcon->ses);
	int flags = CIFS_CP_CREATE_CLOSE_OP;
	struct smb_rqst rqst[3];
	int resp_buftype[3];
	struct kvec rsp_iov[3];
	struct kvec open_iov[SMB2_CREATE_IOV_SIZE];
	struct kvec io_iov[SMB2_IOCTL_IOV_SIZE];
	struct kvec close_iov[1];
	struct smb2_ioctl_rsp *ioctl_rsp;
	struct reparse_data_buffer *reparse_buf;
	u32 plen;

	cifs_dbg(FYI, "%s: path: %s\n", __func__, full_path);

	if (smb3_encryption_required(tcon))
		flags |= CIFS_TRANSFORM_REQ;

	memset(rqst, 0, sizeof(rqst));
	resp_buftype[0] = resp_buftype[1] = resp_buftype[2] = CIFS_NO_BUFFER;
	memset(rsp_iov, 0, sizeof(rsp_iov));

	utf16_path = cifs_convert_path_to_utf16(full_path, cifs_sb);
	if (!utf16_path)
		return -ENOMEM;

	/*
	 * setup smb2open - TODO add optimization to call cifs_get_readable_path
	 * to see if there is a handle already open that we can use
	 */
	memset(&open_iov, 0, sizeof(open_iov));
	rqst[0].rq_iov = open_iov;
	rqst[0].rq_nvec = SMB2_CREATE_IOV_SIZE;

	memset(&oparms, 0, sizeof(oparms));
	oparms.tcon = tcon;
	oparms.desired_access = FILE_READ_ATTRIBUTES;
	oparms.disposition = FILE_OPEN;
	oparms.create_options = cifs_create_options(cifs_sb, OPEN_REPARSE_POINT);
	oparms.fid = &fid;
	oparms.reconnect = false;

	rc = SMB2_open_init(tcon, server,
			    &rqst[0], &oplock, &oparms, utf16_path);
	if (rc)
		goto query_rp_exit;
	smb2_set_next_command(tcon, &rqst[0]);


	/* IOCTL */
	memset(&io_iov, 0, sizeof(io_iov));
	rqst[1].rq_iov = io_iov;
	rqst[1].rq_nvec = SMB2_IOCTL_IOV_SIZE;

	rc = SMB2_ioctl_init(tcon, server,
			     &rqst[1], COMPOUND_FID,
			     COMPOUND_FID, FSCTL_GET_REPARSE_POINT,
			     true /* is_fctl */, NULL, 0,
			     CIFSMaxBufSize -
			     MAX_SMB2_CREATE_RESPONSE_SIZE -
			     MAX_SMB2_CLOSE_RESPONSE_SIZE);
	if (rc)
		goto query_rp_exit;

	smb2_set_next_command(tcon, &rqst[1]);
	smb2_set_related(&rqst[1]);


	/* Close */
	memset(&close_iov, 0, sizeof(close_iov));
	rqst[2].rq_iov = close_iov;
	rqst[2].rq_nvec = 1;

	rc = SMB2_close_init(tcon, server,
			     &rqst[2], COMPOUND_FID, COMPOUND_FID, false);
	if (rc)
		goto query_rp_exit;

	smb2_set_related(&rqst[2]);

	rc = compound_send_recv(xid, tcon->ses, server,
				flags, 3, rqst,
				resp_buftype, rsp_iov);

	ioctl_rsp = rsp_iov[1].iov_base;

	/*
	 * Open was successful and we got an ioctl response.
	 */
	if (rc == 0) {
		/* See MS-FSCC 2.3.23 */

		reparse_buf = (struct reparse_data_buffer *)
			((char *)ioctl_rsp +
			 le32_to_cpu(ioctl_rsp->OutputOffset));
		plen = le32_to_cpu(ioctl_rsp->OutputCount);

		if (plen + le32_to_cpu(ioctl_rsp->OutputOffset) >
		    rsp_iov[1].iov_len) {
			cifs_tcon_dbg(FYI, "srv returned invalid ioctl len: %d\n",
				 plen);
			rc = -EIO;
			goto query_rp_exit;
		}
		*tag = le32_to_cpu(reparse_buf->ReparseTag);
	}

 query_rp_exit:
	kfree(utf16_path);
	SMB2_open_free(&rqst[0]);
	SMB2_ioctl_free(&rqst[1]);
	SMB2_close_free(&rqst[2]);
	free_rsp_buf(resp_buftype[0], rsp_iov[0].iov_base);
	free_rsp_buf(resp_buftype[1], rsp_iov[1].iov_base);
	free_rsp_buf(resp_buftype[2], rsp_iov[2].iov_base);
	return rc;
}