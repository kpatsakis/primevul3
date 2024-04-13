struct dentry *d_splice_alias(struct inode *inode, struct dentry *dentry)
{
	if (IS_ERR(inode))
		return ERR_CAST(inode);

	BUG_ON(!d_unhashed(dentry));

	if (!inode) {
		__d_instantiate(dentry, NULL);
		goto out;
	}
	spin_lock(&inode->i_lock);
	if (S_ISDIR(inode->i_mode)) {
		struct dentry *new = __d_find_any_alias(inode);
		if (unlikely(new)) {
			write_seqlock(&rename_lock);
			if (unlikely(d_ancestor(new, dentry))) {
				write_sequnlock(&rename_lock);
				spin_unlock(&inode->i_lock);
				dput(new);
				new = ERR_PTR(-ELOOP);
				pr_warn_ratelimited(
					"VFS: Lookup of '%s' in %s %s"
					" would have caused loop\n",
					dentry->d_name.name,
					inode->i_sb->s_type->name,
					inode->i_sb->s_id);
			} else if (!IS_ROOT(new)) {
				int err = __d_unalias(inode, dentry, new);
				write_sequnlock(&rename_lock);
				if (err) {
					dput(new);
					new = ERR_PTR(err);
				}
			} else {
				__d_move(new, dentry, false);
				write_sequnlock(&rename_lock);
				spin_unlock(&inode->i_lock);
				security_d_instantiate(new, inode);
			}
			iput(inode);
			return new;
		}
	}
	/* already taking inode->i_lock, so d_add() by hand */
	__d_instantiate(dentry, inode);
	spin_unlock(&inode->i_lock);
out:
	security_d_instantiate(dentry, inode);
	d_rehash(dentry);
	return NULL;
}
