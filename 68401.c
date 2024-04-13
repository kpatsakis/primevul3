static ssize_t loop_attr_dio_show(struct loop_device *lo, char *buf)
{
	int dio = (lo->lo_flags & LO_FLAGS_DIRECT_IO);

	return sprintf(buf, "%s\n", dio ? "1" : "0");
}
