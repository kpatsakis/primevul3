smb2_set_lease_key(struct inode *inode, struct cifs_fid *fid)
{
	memcpy(CIFS_I(inode)->lease_key, fid->lease_key, SMB2_LEASE_KEY_SIZE);
}