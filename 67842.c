static void hwsim_mon_setup(struct net_device *dev)
{
	dev->netdev_ops = &hwsim_netdev_ops;
	dev->needs_free_netdev = true;
	ether_setup(dev);
	dev->priv_flags |= IFF_NO_QUEUE;
	dev->type = ARPHRD_IEEE80211_RADIOTAP;
	eth_zero_addr(dev->dev_addr);
	dev->dev_addr[0] = 0x12;
}
