loop_set_status_compat(struct loop_device *lo,
		       const struct compat_loop_info __user *arg)
{
	struct loop_info64 info64;
	int ret;

	ret = loop_info64_from_compat(arg, &info64);
	if (ret < 0)
		return ret;
	return loop_set_status(lo, &info64);
}
