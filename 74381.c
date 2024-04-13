static inline int hidg_get_minor(void)
{
	int ret;

	ret = ida_simple_get(&hidg_ida, 0, 0, GFP_KERNEL);
	if (ret >= HIDG_MINORS) {
		ida_simple_remove(&hidg_ida, ret);
		ret = -ENODEV;
	}

	return ret;
}
