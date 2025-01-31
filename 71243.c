static ssize_t dbg_lvl_store(struct device_driver *dd, const char *buf,
			     size_t count)
{
	int retval = count;

	if (sscanf(buf, "%u", &megasas_dbg_lvl) < 1) {
		printk(KERN_ERR "megasas: could not set dbg_lvl\n");
		retval = -EINVAL;
	}
	return retval;
}
