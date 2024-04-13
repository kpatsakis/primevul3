smb2_query_dir_next(const unsigned int xid, struct cifs_tcon *tcon,
		    struct cifs_fid *fid, __u16 search_flags,
		    struct cifs_search_info *srch_inf)
{
	return SMB2_query_directory(xid, tcon, fid->persistent_fid,
				    fid->volatile_fid, 0, srch_inf);
}