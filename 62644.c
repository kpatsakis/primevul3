void ovl_path_lower(struct dentry *dentry, struct path *path)
{
	struct ovl_fs *ofs = dentry->d_sb->s_fs_info;
	struct ovl_entry *oe = dentry->d_fsdata;

	path->mnt = ofs->lower_mnt;
	path->dentry = oe->lowerdentry;
}
