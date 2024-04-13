smb2_query_symlink(const unsigned int xid, struct cifs_tcon *tcon,
		   struct cifs_sb_info *cifs_sb, const char *full_path,
		   char **target_path, bool is_reparse_point)
{
	int rc;
	__le16 *utf16_path = NULL;
	__u8 oplock = SMB2_OPLOCK_LEVEL_NONE;
	struct cifs_open_parms oparms;
	struct cifs_fid fid;
	struct kvec err_iov = {NULL, 0};
	struct smb2_err_rsp *err_buf = NULL;
	struct smb2_symlink_err_rsp *symlink;
	struct TCP_Server_Info *server = cifs_pick_channel(tcon->ses);
	unsigned int sub_len;
	unsigned int sub_offset;
	unsigned int print_len;
	unsigned int print_offset;
	int flags = CIFS_CP_CREATE_CLOSE_OP;
	struct smb_rqst rqst[3];
	int resp_buftype[3];
	struct kvec rsp_iov[3];
	struct kvec open_iov[SMB2_CREATE_IOV_SIZE];
	struct kvec io_iov[SMB2_IOCTL_IOV_SIZE];
	struct kvec close_iov[1];
	struct smb2_create_rsp *create_rsp;
	struct smb2_ioctl_rsp *ioctl_rsp;
	struct reparse_data_buffer *reparse_buf;
	int create_options = is_reparse_point ? OPEN_REPARSE_POINT : 0;
	u32 plen;

	cifs_dbg(FYI, "%s: path: %s\n", __func__, full_path);

	*target_path = NULL;

	if (smb3_encryption_required(tcon))
		flags |= CIFS_TRANSFORM_REQ;

	memset(rqst, 0, sizeof(rqst));
	resp_buftype[0] = resp_buftype[1] = resp_buftype[2] = CIFS_NO_BUFFER;
	memset(rsp_iov, 0, sizeof(rsp_iov));

	utf16_path = cifs_convert_path_to_utf16(full_path, cifs_sb);
	if (!utf16_path)
		return -ENOMEM;

	/* Open */
	memset(&open_iov, 0, sizeof(open_iov));
	rqst[0].rq_iov = open_iov;
	rqst[0].rq_nvec = SMB2_CREATE_IOV_SIZE;

	memset(&oparms, 0, sizeof(oparms));
	oparms.tcon = tcon;
	oparms.desired_access = FILE_READ_ATTRIBUTES;
	oparms.disposition = FILE_OPEN;
	oparms.create_options = cifs_create_options(cifs_sb, create_options);
	oparms.fid = &fid;
	oparms.reconnect = false;

	rc = SMB2_open_init(tcon, server,
			    &rqst[0], &oplock, &oparms, utf16_path);
	if (rc)
		goto querty_exit;
	smb2_set_next_command(tcon, &rqst[0]);


	/* IOCTL */
	memset(&io_iov, 0, sizeof(io_iov));
	rqst[1].rq_iov = io_iov;
	rqst[1].rq_nvec = SMB2_IOCTL_IOV_SIZE;

	rc = SMB2_ioctl_init(tcon, server,
			     &rqst[1], fid.persistent_fid,
			     fid.volatile_fid, FSCTL_GET_REPARSE_POINT,
			     true /* is_fctl */, NULL, 0,
			     CIFSMaxBufSize -
			     MAX_SMB2_CREATE_RESPONSE_SIZE -
			     MAX_SMB2_CLOSE_RESPONSE_SIZE);
	if (rc)
		goto querty_exit;

	smb2_set_next_command(tcon, &rqst[1]);
	smb2_set_related(&rqst[1]);


	/* Close */
	memset(&close_iov, 0, sizeof(close_iov));
	rqst[2].rq_iov = close_iov;
	rqst[2].rq_nvec = 1;

	rc = SMB2_close_init(tcon, server,
			     &rqst[2], COMPOUND_FID, COMPOUND_FID, false);
	if (rc)
		goto querty_exit;

	smb2_set_related(&rqst[2]);

	rc = compound_send_recv(xid, tcon->ses, server,
				flags, 3, rqst,
				resp_buftype, rsp_iov);

	create_rsp = rsp_iov[0].iov_base;
	if (create_rsp && create_rsp->hdr.Status)
		err_iov = rsp_iov[0];
	ioctl_rsp = rsp_iov[1].iov_base;

	/*
	 * Open was successful and we got an ioctl response.
	 */
	if ((rc == 0) && (is_reparse_point)) {
		/* See MS-FSCC 2.3.23 */

		reparse_buf = (struct reparse_data_buffer *)
			((char *)ioctl_rsp +
			 le32_to_cpu(ioctl_rsp->OutputOffset));
		plen = le32_to_cpu(ioctl_rsp->OutputCount);

		if (plen + le32_to_cpu(ioctl_rsp->OutputOffset) >
		    rsp_iov[1].iov_len) {
			cifs_tcon_dbg(VFS, "srv returned invalid ioctl len: %d\n",
				 plen);
			rc = -EIO;
			goto querty_exit;
		}

		rc = parse_reparse_point(reparse_buf, plen, target_path,
					 cifs_sb);
		goto querty_exit;
	}

	if (!rc || !err_iov.iov_base) {
		rc = -ENOENT;
		goto querty_exit;
	}

	err_buf = err_iov.iov_base;
	if (le32_to_cpu(err_buf->ByteCount) < sizeof(struct smb2_symlink_err_rsp) ||
	    err_iov.iov_len < SMB2_SYMLINK_STRUCT_SIZE) {
		rc = -EINVAL;
		goto querty_exit;
	}

	symlink = (struct smb2_symlink_err_rsp *)err_buf->ErrorData;
	if (le32_to_cpu(symlink->SymLinkErrorTag) != SYMLINK_ERROR_TAG ||
	    le32_to_cpu(symlink->ReparseTag) != IO_REPARSE_TAG_SYMLINK) {
		rc = -EINVAL;
		goto querty_exit;
	}

	/* open must fail on symlink - reset rc */
	rc = 0;
	sub_len = le16_to_cpu(symlink->SubstituteNameLength);
	sub_offset = le16_to_cpu(symlink->SubstituteNameOffset);
	print_len = le16_to_cpu(symlink->PrintNameLength);
	print_offset = le16_to_cpu(symlink->PrintNameOffset);

	if (err_iov.iov_len < SMB2_SYMLINK_STRUCT_SIZE + sub_offset + sub_len) {
		rc = -EINVAL;
		goto querty_exit;
	}

	if (err_iov.iov_len <
	    SMB2_SYMLINK_STRUCT_SIZE + print_offset + print_len) {
		rc = -EINVAL;
		goto querty_exit;
	}

	*target_path = cifs_strndup_from_utf16(
				(char *)symlink->PathBuffer + sub_offset,
				sub_len, true, cifs_sb->local_nls);
	if (!(*target_path)) {
		rc = -ENOMEM;
		goto querty_exit;
	}
	convert_delimiter(*target_path, '/');
	cifs_dbg(FYI, "%s: target path: %s\n", __func__, *target_path);

 querty_exit:
	cifs_dbg(FYI, "query symlink rc %d\n", rc);
	kfree(utf16_path);
	SMB2_open_free(&rqst[0]);
	SMB2_ioctl_free(&rqst[1]);
	SMB2_close_free(&rqst[2]);
	free_rsp_buf(resp_buftype[0], rsp_iov[0].iov_base);
	free_rsp_buf(resp_buftype[1], rsp_iov[1].iov_base);
	free_rsp_buf(resp_buftype[2], rsp_iov[2].iov_base);
	return rc;
}