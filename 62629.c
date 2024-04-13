void ovl_dentry_update(struct dentry *dentry, struct dentry *upperdentry)
{
	struct ovl_entry *oe = dentry->d_fsdata;

	WARN_ON(!mutex_is_locked(&upperdentry->d_parent->d_inode->i_mutex));
	WARN_ON(oe->__upperdentry);
	BUG_ON(!upperdentry->d_inode);
	/*
	 * Make sure upperdentry is consistent before making it visible to
	 * ovl_upperdentry_dereference().
	 */
	smp_wmb();
	oe->__upperdentry = upperdentry;
}
