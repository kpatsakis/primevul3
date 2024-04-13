smb3_set_integrity(const unsigned int xid, struct cifs_tcon *tcon,
		   struct cifsFileInfo *cfile)
{
	struct fsctl_set_integrity_information_req integr_info;
	unsigned int ret_data_len;

	integr_info.ChecksumAlgorithm = cpu_to_le16(CHECKSUM_TYPE_UNCHANGED);
	integr_info.Flags = 0;
	integr_info.Reserved = 0;

	return SMB2_ioctl(xid, tcon, cfile->fid.persistent_fid,
			cfile->fid.volatile_fid,
			FSCTL_SET_INTEGRITY_INFORMATION,
			true /* is_fsctl */,
			(char *)&integr_info,
			sizeof(struct fsctl_set_integrity_information_req),
			CIFSMaxBufSize, NULL,
			&ret_data_len);

}