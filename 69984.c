static int ipip6_changelink(struct net_device *dev, struct nlattr *tb[],
			    struct nlattr *data[],
			    struct netlink_ext_ack *extack)
{
	struct ip_tunnel *t = netdev_priv(dev);
	struct ip_tunnel_parm p;
	struct ip_tunnel_encap ipencap;
	struct net *net = t->net;
	struct sit_net *sitn = net_generic(net, sit_net_id);
#ifdef CONFIG_IPV6_SIT_6RD
	struct ip_tunnel_6rd ip6rd;
#endif
	__u32 fwmark = t->fwmark;
	int err;

	if (dev == sitn->fb_tunnel_dev)
		return -EINVAL;

	if (ipip6_netlink_encap_parms(data, &ipencap)) {
		err = ip_tunnel_encap_setup(t, &ipencap);
		if (err < 0)
			return err;
	}

	ipip6_netlink_parms(data, &p, &fwmark);

	if (((dev->flags & IFF_POINTOPOINT) && !p.iph.daddr) ||
	    (!(dev->flags & IFF_POINTOPOINT) && p.iph.daddr))
		return -EINVAL;

	t = ipip6_tunnel_locate(net, &p, 0);

	if (t) {
		if (t->dev != dev)
			return -EEXIST;
	} else
		t = netdev_priv(dev);

	ipip6_tunnel_update(t, &p, fwmark);

#ifdef CONFIG_IPV6_SIT_6RD
	if (ipip6_netlink_6rd_parms(data, &ip6rd))
		return ipip6_tunnel_update_6rd(t, &ip6rd);
#endif

	return 0;
}
