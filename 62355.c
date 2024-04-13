static int error_inject_set(void *data, u64 val)
{
	if (!error_type)
		return -EINVAL;

	return einj_error_inject(error_type, error_flags, error_param1, error_param2,
		error_param3, error_param4);
}
