static int loop_set_dio(struct loop_device *lo, unsigned long arg)
{
	int error = -ENXIO;
	if (lo->lo_state != Lo_bound)
		goto out;

	__loop_update_dio(lo, !!arg);
	if (lo->use_dio == !!arg)
		return 0;
	error = -EINVAL;
 out:
	return error;
}
