static int ovl_mount_dir(const char *name, struct path *path)
{
	int err;

	err = kern_path(name, LOOKUP_FOLLOW, path);
	if (err) {
		pr_err("overlayfs: failed to resolve '%s': %i\n", name, err);
		err = -EINVAL;
	}
	return err;
}
