static int __d_unalias(struct inode *inode,
		struct dentry *dentry, struct dentry *alias)
{
	struct mutex *m1 = NULL, *m2 = NULL;
	int ret = -ESTALE;

	/* If alias and dentry share a parent, then no extra locks required */
	if (alias->d_parent == dentry->d_parent)
		goto out_unalias;

	/* See lock_rename() */
	if (!mutex_trylock(&dentry->d_sb->s_vfs_rename_mutex))
		goto out_err;
	m1 = &dentry->d_sb->s_vfs_rename_mutex;
	if (!mutex_trylock(&alias->d_parent->d_inode->i_mutex))
		goto out_err;
	m2 = &alias->d_parent->d_inode->i_mutex;
out_unalias:
	__d_move(alias, dentry, false);
	ret = 0;
out_err:
	spin_unlock(&inode->i_lock);
	if (m2)
		mutex_unlock(m2);
	if (m1)
		mutex_unlock(m1);
	return ret;
}
