smb2_close_getattr(const unsigned int xid, struct cifs_tcon *tcon,
		   struct cifsFileInfo *cfile)
{
	struct smb2_file_network_open_info file_inf;
	struct inode *inode;
	int rc;

	rc = __SMB2_close(xid, tcon, cfile->fid.persistent_fid,
		   cfile->fid.volatile_fid, &file_inf);
	if (rc)
		return;

	inode = d_inode(cfile->dentry);

	spin_lock(&inode->i_lock);
	CIFS_I(inode)->time = jiffies;

	/* Creation time should not need to be updated on close */
	if (file_inf.LastWriteTime)
		inode->i_mtime = cifs_NTtimeToUnix(file_inf.LastWriteTime);
	if (file_inf.ChangeTime)
		inode->i_ctime = cifs_NTtimeToUnix(file_inf.ChangeTime);
	if (file_inf.LastAccessTime)
		inode->i_atime = cifs_NTtimeToUnix(file_inf.LastAccessTime);

	/*
	 * i_blocks is not related to (i_size / i_blksize),
	 * but instead 512 byte (2**9) size is required for
	 * calculating num blocks.
	 */
	if (le64_to_cpu(file_inf.AllocationSize) > 4096)
		inode->i_blocks =
			(512 - 1 + le64_to_cpu(file_inf.AllocationSize)) >> 9;

	/* End of file and Attributes should not have to be updated on close */
	spin_unlock(&inode->i_lock);
}