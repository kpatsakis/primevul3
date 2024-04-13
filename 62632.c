void ovl_dentry_version_inc(struct dentry *dentry)
{
	struct ovl_entry *oe = dentry->d_fsdata;

	WARN_ON(!mutex_is_locked(&dentry->d_inode->i_mutex));
	oe->version++;
}
