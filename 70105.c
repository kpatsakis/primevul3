void genl_lock(void)
{
	mutex_lock(&genl_mutex);
}
