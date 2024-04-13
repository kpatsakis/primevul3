struct dentry *d_make_root(struct inode *root_inode)
{
	struct dentry *res = NULL;

	if (root_inode) {
		static const struct qstr name = QSTR_INIT("/", 1);

		res = __d_alloc(root_inode->i_sb, &name);
		if (res)
			d_instantiate(res, root_inode);
		else
			iput(root_inode);
	}
	return res;
}
