static bool ovl_is_opaquedir(struct dentry *dentry)
{
	int res;
	char val;
	struct inode *inode = dentry->d_inode;

	if (!S_ISDIR(inode->i_mode) || !inode->i_op->getxattr)
		return false;

	res = inode->i_op->getxattr(dentry, ovl_opaque_xattr, &val, 1);
	if (res == 1 && val == 'y')
		return true;

	return false;
}
