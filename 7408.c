smb2_flush_file(const unsigned int xid, struct cifs_tcon *tcon,
		struct cifs_fid *fid)
{
	return SMB2_flush(xid, tcon, fid->persistent_fid, fid->volatile_fid);
}