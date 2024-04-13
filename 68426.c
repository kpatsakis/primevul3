int loop_register_transfer(struct loop_func_table *funcs)
{
	unsigned int n = funcs->number;

	if (n >= MAX_LO_CRYPT || xfer_funcs[n])
		return -EINVAL;
	xfer_funcs[n] = funcs;
	return 0;
}
