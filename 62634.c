void ovl_drop_write(struct dentry *dentry)
{
	struct ovl_fs *ofs = dentry->d_sb->s_fs_info;
	mnt_drop_write(ofs->upper_mnt);
}
