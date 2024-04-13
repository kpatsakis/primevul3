static int loop_exit_cb(int id, void *ptr, void *data)
{
	struct loop_device *lo = ptr;

	loop_remove(lo);
	return 0;
}
