bool lockdep_genl_is_held(void)
{
	return lockdep_is_held(&genl_mutex);
}
