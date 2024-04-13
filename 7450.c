smb2_set_compression(const unsigned int xid, struct cifs_tcon *tcon,
		   struct cifsFileInfo *cfile)
{
	return SMB2_set_compression(xid, tcon, cfile->fid.persistent_fid,
			    cfile->fid.volatile_fid);
}