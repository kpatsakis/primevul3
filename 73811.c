struct dentry *d_add_ci(struct dentry *dentry, struct inode *inode,
			struct qstr *name)
{
	struct dentry *found;
	struct dentry *new;

	/*
	 * First check if a dentry matching the name already exists,
	 * if not go ahead and create it now.
	 */
	found = d_hash_and_lookup(dentry->d_parent, name);
	if (!found) {
		new = d_alloc(dentry->d_parent, name);
		if (!new) {
			found = ERR_PTR(-ENOMEM);
		} else {
			found = d_splice_alias(inode, new);
			if (found) {
				dput(new);
				return found;
			}
			return new;
		}
	}
	iput(inode);
	return found;
}
