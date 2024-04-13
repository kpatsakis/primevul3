static struct dquot **f2fs_get_dquots(struct inode *inode)
{
	return F2FS_I(inode)->i_dquot;
}
