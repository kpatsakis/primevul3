static int inode_has_encryption_context(struct inode *inode)
{
	if (!inode->i_sb->s_cop->get_context)
		return 0;
	return (inode->i_sb->s_cop->get_context(inode, NULL, 0L) > 0);
}
