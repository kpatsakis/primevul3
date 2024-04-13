static void tun_net_uninit(struct net_device *dev)
{
	tun_detach_all(dev);
}
