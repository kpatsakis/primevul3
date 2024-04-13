static void __net_init ipip6_fb_tunnel_init(struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct iphdr *iph = &tunnel->parms.iph;
	struct net *net = dev_net(dev);
	struct sit_net *sitn = net_generic(net, sit_net_id);

	iph->version		= 4;
	iph->protocol		= IPPROTO_IPV6;
	iph->ihl		= 5;
	iph->ttl		= 64;

	dev_hold(dev);
	rcu_assign_pointer(sitn->tunnels_wc[0], tunnel);
}
