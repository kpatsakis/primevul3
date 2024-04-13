static ssize_t loop_attr_offset_show(struct loop_device *lo, char *buf)
{
	return sprintf(buf, "%llu\n", (unsigned long long)lo->lo_offset);
}
