static struct dentry *__d_obtain_alias(struct inode *inode, int disconnected)
{
	static const struct qstr anonstring = QSTR_INIT("/", 1);
	struct dentry *tmp;
	struct dentry *res;
	unsigned add_flags;

	if (!inode)
		return ERR_PTR(-ESTALE);
	if (IS_ERR(inode))
		return ERR_CAST(inode);

	res = d_find_any_alias(inode);
	if (res)
		goto out_iput;

	tmp = __d_alloc(inode->i_sb, &anonstring);
	if (!tmp) {
		res = ERR_PTR(-ENOMEM);
		goto out_iput;
	}

	spin_lock(&inode->i_lock);
	res = __d_find_any_alias(inode);
	if (res) {
		spin_unlock(&inode->i_lock);
		dput(tmp);
		goto out_iput;
	}

	/* attach a disconnected dentry */
	add_flags = d_flags_for_inode(inode);

	if (disconnected)
		add_flags |= DCACHE_DISCONNECTED;

	spin_lock(&tmp->d_lock);
	__d_set_inode_and_type(tmp, inode, add_flags);
	hlist_add_head(&tmp->d_u.d_alias, &inode->i_dentry);
	hlist_bl_lock(&tmp->d_sb->s_anon);
	hlist_bl_add_head(&tmp->d_hash, &tmp->d_sb->s_anon);
	hlist_bl_unlock(&tmp->d_sb->s_anon);
	spin_unlock(&tmp->d_lock);
	spin_unlock(&inode->i_lock);
	security_d_instantiate(tmp, inode);

	return tmp;

 out_iput:
	if (res && !IS_ERR(res))
		security_d_instantiate(res, inode);
	iput(inode);
	return res;
}
