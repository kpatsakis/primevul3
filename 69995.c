static void ipip6_tunnel_bind_dev(struct net_device *dev)
{
	struct net_device *tdev = NULL;
	struct ip_tunnel *tunnel;
	const struct iphdr *iph;
	struct flowi4 fl4;

	tunnel = netdev_priv(dev);
	iph = &tunnel->parms.iph;

	if (iph->daddr) {
		struct rtable *rt = ip_route_output_ports(tunnel->net, &fl4,
							  NULL,
							  iph->daddr, iph->saddr,
							  0, 0,
							  IPPROTO_IPV6,
							  RT_TOS(iph->tos),
							  tunnel->parms.link);

		if (!IS_ERR(rt)) {
			tdev = rt->dst.dev;
			ip_rt_put(rt);
		}
		dev->flags |= IFF_POINTOPOINT;
	}

	if (!tdev && tunnel->parms.link)
		tdev = __dev_get_by_index(tunnel->net, tunnel->parms.link);

	if (tdev) {
		int t_hlen = tunnel->hlen + sizeof(struct iphdr);

		dev->hard_header_len = tdev->hard_header_len + sizeof(struct iphdr);
		dev->mtu = tdev->mtu - t_hlen;
		if (dev->mtu < IPV6_MIN_MTU)
			dev->mtu = IPV6_MIN_MTU;
	}
}
