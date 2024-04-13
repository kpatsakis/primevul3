xfs_init_security(
	struct inode	*inode,
	struct inode	*dir,
	const struct qstr *qstr)
{
	return security_inode_init_security(inode, dir, qstr,
					     &xfs_initxattrs, NULL);
}
