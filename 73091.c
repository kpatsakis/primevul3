static void tun_setup(struct net_device *dev)
{
	struct tun_struct *tun = netdev_priv(dev);

	tun->owner = INVALID_UID;
	tun->group = INVALID_GID;

	dev->ethtool_ops = &tun_ethtool_ops;
	dev->needs_free_netdev = true;
	dev->priv_destructor = tun_free_netdev;
	/* We prefer our own queue length */
	dev->tx_queue_len = TUN_READQ_SIZE;
}
