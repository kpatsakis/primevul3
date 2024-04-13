static void ipip6_tunnel_setup(struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	int t_hlen = tunnel->hlen + sizeof(struct iphdr);

	dev->netdev_ops		= &ipip6_netdev_ops;
	dev->needs_free_netdev	= true;
	dev->priv_destructor	= ipip6_dev_free;

	dev->type		= ARPHRD_SIT;
	dev->hard_header_len	= LL_MAX_HEADER + t_hlen;
	dev->mtu		= ETH_DATA_LEN - t_hlen;
	dev->min_mtu		= IPV6_MIN_MTU;
	dev->max_mtu		= IP6_MAX_MTU - t_hlen;
	dev->flags		= IFF_NOARP;
	netif_keep_dst(dev);
	dev->addr_len		= 4;
	dev->features		|= NETIF_F_LLTX;
	dev->features		|= SIT_FEATURES;
	dev->hw_features	|= SIT_FEATURES;
}
