smb2_queryfs(const unsigned int xid, struct cifs_tcon *tcon,
	     struct cifs_sb_info *cifs_sb, struct kstatfs *buf)
{
	struct smb2_query_info_rsp *rsp;
	struct smb2_fs_full_size_info *info = NULL;
	struct kvec rsp_iov = {NULL, 0};
	int buftype = CIFS_NO_BUFFER;
	int rc;


	rc = smb2_query_info_compound(xid, tcon, "",
				      FILE_READ_ATTRIBUTES,
				      FS_FULL_SIZE_INFORMATION,
				      SMB2_O_INFO_FILESYSTEM,
				      sizeof(struct smb2_fs_full_size_info),
				      &rsp_iov, &buftype, cifs_sb);
	if (rc)
		goto qfs_exit;

	rsp = (struct smb2_query_info_rsp *)rsp_iov.iov_base;
	buf->f_type = SMB2_SUPER_MAGIC;
	info = (struct smb2_fs_full_size_info *)(
		le16_to_cpu(rsp->OutputBufferOffset) + (char *)rsp);
	rc = smb2_validate_iov(le16_to_cpu(rsp->OutputBufferOffset),
			       le32_to_cpu(rsp->OutputBufferLength),
			       &rsp_iov,
			       sizeof(struct smb2_fs_full_size_info));
	if (!rc)
		smb2_copy_fs_info_to_kstatfs(info, buf);

qfs_exit:
	free_rsp_buf(buftype, rsp_iov.iov_base);
	return rc;
}