static void ipip6_tunnel_uninit(struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct sit_net *sitn = net_generic(tunnel->net, sit_net_id);

	if (dev == sitn->fb_tunnel_dev) {
		RCU_INIT_POINTER(sitn->tunnels_wc[0], NULL);
	} else {
		ipip6_tunnel_unlink(sitn, tunnel);
		ipip6_tunnel_del_prl(tunnel, NULL);
	}
	dst_cache_reset(&tunnel->dst_cache);
	dev_put(dev);
}
