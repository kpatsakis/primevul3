smb2_sync_write(const unsigned int xid, struct cifs_fid *pfid,
		struct cifs_io_parms *parms, unsigned int *written,
		struct kvec *iov, unsigned long nr_segs)
{

	parms->persistent_fid = pfid->persistent_fid;
	parms->volatile_fid = pfid->volatile_fid;
	return SMB2_write(xid, parms, written, iov, nr_segs);
}