static ssize_t loop_attr_partscan_show(struct loop_device *lo, char *buf)
{
	int partscan = (lo->lo_flags & LO_FLAGS_PARTSCAN);

	return sprintf(buf, "%s\n", partscan ? "1" : "0");
}
