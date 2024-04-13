static struct ip_tunnel *ipip6_tunnel_lookup(struct net *net,
					     struct net_device *dev,
					     __be32 remote, __be32 local,
					     int sifindex)
{
	unsigned int h0 = HASH(remote);
	unsigned int h1 = HASH(local);
	struct ip_tunnel *t;
	struct sit_net *sitn = net_generic(net, sit_net_id);
	int ifindex = dev ? dev->ifindex : 0;

	for_each_ip_tunnel_rcu(t, sitn->tunnels_r_l[h0 ^ h1]) {
		if (local == t->parms.iph.saddr &&
		    remote == t->parms.iph.daddr &&
		    (!dev || !t->parms.link || ifindex == t->parms.link ||
		     sifindex == t->parms.link) &&
		    (t->dev->flags & IFF_UP))
			return t;
	}
	for_each_ip_tunnel_rcu(t, sitn->tunnels_r[h0]) {
		if (remote == t->parms.iph.daddr &&
		    (!dev || !t->parms.link || ifindex == t->parms.link ||
		     sifindex == t->parms.link) &&
		    (t->dev->flags & IFF_UP))
			return t;
	}
	for_each_ip_tunnel_rcu(t, sitn->tunnels_l[h1]) {
		if (local == t->parms.iph.saddr &&
		    (!dev || !t->parms.link || ifindex == t->parms.link ||
		     sifindex == t->parms.link) &&
		    (t->dev->flags & IFF_UP))
			return t;
	}
	t = rcu_dereference(sitn->tunnels_wc[0]);
	if (t && (t->dev->flags & IFF_UP))
		return t;
	return NULL;
}
