static int ipip6_tunnel_create(struct net_device *dev)
{
	struct ip_tunnel *t = netdev_priv(dev);
	struct net *net = dev_net(dev);
	struct sit_net *sitn = net_generic(net, sit_net_id);
	int err;

	memcpy(dev->dev_addr, &t->parms.iph.saddr, 4);
	memcpy(dev->broadcast, &t->parms.iph.daddr, 4);

	if ((__force u16)t->parms.i_flags & SIT_ISATAP)
		dev->priv_flags |= IFF_ISATAP;

	dev->rtnl_link_ops = &sit_link_ops;

	err = register_netdevice(dev);
	if (err < 0)
		goto out;

	ipip6_tunnel_clone_6rd(dev, sitn);

	dev_hold(dev);

	ipip6_tunnel_link(sitn, t);
	return 0;

out:
	return err;
}
