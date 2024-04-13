static enum d_walk_ret check_mount(void *data, struct dentry *dentry)
{
	int *ret = data;
	if (d_mountpoint(dentry)) {
		*ret = 1;
		return D_WALK_QUIT;
	}
	return D_WALK_CONTINUE;
}
