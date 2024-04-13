static loff_t get_loop_size(struct loop_device *lo, struct file *file)
{
	return get_size(lo->lo_offset, lo->lo_sizelimit, file);
}
