smb2_query_eas(const unsigned int xid, struct cifs_tcon *tcon,
	       const unsigned char *path, const unsigned char *ea_name,
	       char *ea_data, size_t buf_size,
	       struct cifs_sb_info *cifs_sb)
{
	int rc;
	struct kvec rsp_iov = {NULL, 0};
	int buftype = CIFS_NO_BUFFER;
	struct smb2_query_info_rsp *rsp;
	struct smb2_file_full_ea_info *info = NULL;

	rc = smb2_query_info_compound(xid, tcon, path,
				      FILE_READ_EA,
				      FILE_FULL_EA_INFORMATION,
				      SMB2_O_INFO_FILE,
				      CIFSMaxBufSize -
				      MAX_SMB2_CREATE_RESPONSE_SIZE -
				      MAX_SMB2_CLOSE_RESPONSE_SIZE,
				      &rsp_iov, &buftype, cifs_sb);
	if (rc) {
		/*
		 * If ea_name is NULL (listxattr) and there are no EAs,
		 * return 0 as it's not an error. Otherwise, the specified
		 * ea_name was not found.
		 */
		if (!ea_name && rc == -ENODATA)
			rc = 0;
		goto qeas_exit;
	}

	rsp = (struct smb2_query_info_rsp *)rsp_iov.iov_base;
	rc = smb2_validate_iov(le16_to_cpu(rsp->OutputBufferOffset),
			       le32_to_cpu(rsp->OutputBufferLength),
			       &rsp_iov,
			       sizeof(struct smb2_file_full_ea_info));
	if (rc)
		goto qeas_exit;

	info = (struct smb2_file_full_ea_info *)(
			le16_to_cpu(rsp->OutputBufferOffset) + (char *)rsp);
	rc = move_smb2_ea_to_cifs(ea_data, buf_size, info,
			le32_to_cpu(rsp->OutputBufferLength), ea_name);

 qeas_exit:
	free_rsp_buf(buftype, rsp_iov.iov_base);
	return rc;
}