int have_submounts(struct dentry *parent)
{
	int ret = 0;

	d_walk(parent, &ret, check_mount, NULL);

	return ret;
}
