int f2fs_get_projid(struct inode *inode, kprojid_t *projid)
{
	*projid = F2FS_I(inode)->i_projid;
	return 0;
}
