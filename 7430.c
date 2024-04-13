smb2_get_srv_inum(const unsigned int xid, struct cifs_tcon *tcon,
		  struct cifs_sb_info *cifs_sb, const char *full_path,
		  u64 *uniqueid, FILE_ALL_INFO *data)
{
	*uniqueid = le64_to_cpu(data->IndexNumber);
	return 0;
}