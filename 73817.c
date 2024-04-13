void d_instantiate(struct dentry *entry, struct inode * inode)
{
	BUG_ON(!hlist_unhashed(&entry->d_u.d_alias));
	if (inode)
		spin_lock(&inode->i_lock);
	__d_instantiate(entry, inode);
	if (inode)
		spin_unlock(&inode->i_lock);
	security_d_instantiate(entry, inode);
}
