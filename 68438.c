static inline void loop_update_dio(struct loop_device *lo)
{
	__loop_update_dio(lo, io_is_direct(lo->lo_backing_file) |
			lo->use_dio);
}
