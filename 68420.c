loop_init_xfer(struct loop_device *lo, struct loop_func_table *xfer,
	       const struct loop_info64 *i)
{
	int err = 0;

	if (xfer) {
		struct module *owner = xfer->owner;

		if (!try_module_get(owner))
			return -EINVAL;
		if (xfer->init)
			err = xfer->init(lo, i);
		if (err)
			module_put(owner);
		else
			lo->lo_encryption = xfer;
	}
	return err;
}
