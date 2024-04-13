static void tun_detach(struct tun_file *tfile, bool clean)
{
	rtnl_lock();
	__tun_detach(tfile, clean);
	rtnl_unlock();
}
