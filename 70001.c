static struct ip_tunnel *ipip6_tunnel_locate(struct net *net,
		struct ip_tunnel_parm *parms, int create)
{
	__be32 remote = parms->iph.daddr;
	__be32 local = parms->iph.saddr;
	struct ip_tunnel *t, *nt;
	struct ip_tunnel __rcu **tp;
	struct net_device *dev;
	char name[IFNAMSIZ];
	struct sit_net *sitn = net_generic(net, sit_net_id);

	for (tp = __ipip6_bucket(sitn, parms);
	    (t = rtnl_dereference(*tp)) != NULL;
	     tp = &t->next) {
		if (local == t->parms.iph.saddr &&
		    remote == t->parms.iph.daddr &&
		    parms->link == t->parms.link) {
			if (create)
				return NULL;
			else
				return t;
		}
	}
	if (!create)
		goto failed;

	if (parms->name[0]) {
		if (!dev_valid_name(parms->name))
			goto failed;
		strlcpy(name, parms->name, IFNAMSIZ);
	} else {
		strcpy(name, "sit%d");
	}
	dev = alloc_netdev(sizeof(*t), name, NET_NAME_UNKNOWN,
			   ipip6_tunnel_setup);
	if (!dev)
		return NULL;

	dev_net_set(dev, net);

	nt = netdev_priv(dev);

	nt->parms = *parms;
	if (ipip6_tunnel_create(dev) < 0)
		goto failed_free;

	return nt;

failed_free:
	free_netdev(dev);
failed:
	return NULL;
}
