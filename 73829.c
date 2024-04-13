void dentry_update_name_case(struct dentry *dentry, struct qstr *name)
{
	BUG_ON(!mutex_is_locked(&dentry->d_parent->d_inode->i_mutex));
	BUG_ON(dentry->d_name.len != name->len); /* d_lookup gives this */

	spin_lock(&dentry->d_lock);
	write_seqcount_begin(&dentry->d_seq);
	memcpy((unsigned char *)dentry->d_name.name, name->name, name->len);
	write_seqcount_end(&dentry->d_seq);
	spin_unlock(&dentry->d_lock);
}
