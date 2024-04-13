static int unregister_transfer_cb(int id, void *ptr, void *data)
{
	struct loop_device *lo = ptr;
	struct loop_func_table *xfer = data;

	mutex_lock(&lo->lo_ctl_mutex);
	if (lo->lo_encryption == xfer)
		loop_release_xfer(lo);
	mutex_unlock(&lo->lo_ctl_mutex);
	return 0;
}
