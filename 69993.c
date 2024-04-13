static int ipip6_newlink(struct net *src_net, struct net_device *dev,
			 struct nlattr *tb[], struct nlattr *data[],
			 struct netlink_ext_ack *extack)
{
	struct net *net = dev_net(dev);
	struct ip_tunnel *nt;
	struct ip_tunnel_encap ipencap;
#ifdef CONFIG_IPV6_SIT_6RD
	struct ip_tunnel_6rd ip6rd;
#endif
	int err;

	nt = netdev_priv(dev);

	if (ipip6_netlink_encap_parms(data, &ipencap)) {
		err = ip_tunnel_encap_setup(nt, &ipencap);
		if (err < 0)
			return err;
	}

	ipip6_netlink_parms(data, &nt->parms, &nt->fwmark);

	if (ipip6_tunnel_locate(net, &nt->parms, 0))
		return -EEXIST;

	err = ipip6_tunnel_create(dev);
	if (err < 0)
		return err;

	if (tb[IFLA_MTU]) {
		u32 mtu = nla_get_u32(tb[IFLA_MTU]);

		if (mtu >= IPV6_MIN_MTU &&
		    mtu <= IP6_MAX_MTU - dev->hard_header_len)
			dev->mtu = mtu;
	}

#ifdef CONFIG_IPV6_SIT_6RD
	if (ipip6_netlink_6rd_parms(data, &ip6rd))
		err = ipip6_tunnel_update_6rd(nt, &ip6rd);
#endif

	return err;
}
