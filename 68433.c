loop_set_status64(struct loop_device *lo, const struct loop_info64 __user *arg)
{
	struct loop_info64 info64;

	if (copy_from_user(&info64, arg, sizeof (struct loop_info64)))
		return -EFAULT;
	return loop_set_status(lo, &info64);
}
