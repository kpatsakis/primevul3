static ssize_t loop_attr_autoclear_show(struct loop_device *lo, char *buf)
{
	int autoclear = (lo->lo_flags & LO_FLAGS_AUTOCLEAR);

	return sprintf(buf, "%s\n", autoclear ? "1" : "0");
}
