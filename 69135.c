static qsize_t *f2fs_get_reserved_space(struct inode *inode)
{
	return &F2FS_I(inode)->i_reserved_quota;
}
