smb2_sync_read(const unsigned int xid, struct cifs_fid *pfid,
	       struct cifs_io_parms *parms, unsigned int *bytes_read,
	       char **buf, int *buf_type)
{
	parms->persistent_fid = pfid->persistent_fid;
	parms->volatile_fid = pfid->volatile_fid;
	return SMB2_read(xid, parms, bytes_read, buf, buf_type);
}