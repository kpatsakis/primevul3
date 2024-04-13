static void ipip6_dev_free(struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);

	dst_cache_destroy(&tunnel->dst_cache);
	free_percpu(dev->tstats);
}
