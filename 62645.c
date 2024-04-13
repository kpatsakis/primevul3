struct file *ovl_path_open(struct path *path, int flags)
{
	return dentry_open(path, flags, current_cred());
}
