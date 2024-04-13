static void genl_unlock_all(void)
{
	genl_unlock();
	up_write(&cb_lock);
}
