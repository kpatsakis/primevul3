static void genl_lock_all(void)
{
	down_write(&cb_lock);
	genl_lock();
}
