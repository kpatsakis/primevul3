static int ipip6_tunnel_init(struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	int err;

	tunnel->dev = dev;
	tunnel->net = dev_net(dev);
	strcpy(tunnel->parms.name, dev->name);

	ipip6_tunnel_bind_dev(dev);
	dev->tstats = netdev_alloc_pcpu_stats(struct pcpu_sw_netstats);
	if (!dev->tstats)
		return -ENOMEM;

	err = dst_cache_init(&tunnel->dst_cache, GFP_KERNEL);
	if (err) {
		free_percpu(dev->tstats);
		dev->tstats = NULL;
		return err;
	}

	return 0;
}
