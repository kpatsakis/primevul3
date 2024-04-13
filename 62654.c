int ovl_want_write(struct dentry *dentry)
{
	struct ovl_fs *ofs = dentry->d_sb->s_fs_info;
	return mnt_want_write(ofs->upper_mnt);
}
