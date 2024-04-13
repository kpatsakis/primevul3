smb2_get_lease_key(struct inode *inode, struct cifs_fid *fid)
{
	memcpy(fid->lease_key, CIFS_I(inode)->lease_key, SMB2_LEASE_KEY_SIZE);
}