static inline struct dentry *ovl_lookup_real(struct dentry *dir,
					     struct qstr *name)
{
	struct dentry *dentry;

	mutex_lock(&dir->d_inode->i_mutex);
	dentry = lookup_one_len(name->name, dir, name->len);
	mutex_unlock(&dir->d_inode->i_mutex);

	if (IS_ERR(dentry)) {
		if (PTR_ERR(dentry) == -ENOENT)
			dentry = NULL;
	} else if (!dentry->d_inode) {
		dput(dentry);
		dentry = NULL;
	}
	return dentry;
}
