static int ovl_statfs(struct dentry *dentry, struct kstatfs *buf)
{
	struct ovl_fs *ofs = dentry->d_sb->s_fs_info;
	struct dentry *root_dentry = dentry->d_sb->s_root;
	struct path path;
	int err;

	ovl_path_upper(root_dentry, &path);

	err = vfs_statfs(&path, buf);
	if (!err) {
		buf->f_namelen = max(buf->f_namelen, ofs->lower_namelen);
		buf->f_type = OVERLAYFS_SUPER_MAGIC;
	}

	return err;
}
