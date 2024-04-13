smb2_set_file_size(const unsigned int xid, struct cifs_tcon *tcon,
		   struct cifsFileInfo *cfile, __u64 size, bool set_alloc)
{
	__le64 eof = cpu_to_le64(size);
	struct inode *inode;

	/*
	 * If extending file more than one page make sparse. Many Linux fs
	 * make files sparse by default when extending via ftruncate
	 */
	inode = d_inode(cfile->dentry);

	if (!set_alloc && (size > inode->i_size + 8192)) {
		__u8 set_sparse = 1;

		/* whether set sparse succeeds or not, extend the file */
		smb2_set_sparse(xid, tcon, cfile, inode, set_sparse);
	}

	return SMB2_set_eof(xid, tcon, cfile->fid.persistent_fid,
			    cfile->fid.volatile_fid, cfile->pid, &eof);
}