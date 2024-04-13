static void ipip6_tunnel_update(struct ip_tunnel *t, struct ip_tunnel_parm *p,
				__u32 fwmark)
{
	struct net *net = t->net;
	struct sit_net *sitn = net_generic(net, sit_net_id);

	ipip6_tunnel_unlink(sitn, t);
	synchronize_net();
	t->parms.iph.saddr = p->iph.saddr;
	t->parms.iph.daddr = p->iph.daddr;
	memcpy(t->dev->dev_addr, &p->iph.saddr, 4);
	memcpy(t->dev->broadcast, &p->iph.daddr, 4);
	ipip6_tunnel_link(sitn, t);
	t->parms.iph.ttl = p->iph.ttl;
	t->parms.iph.tos = p->iph.tos;
	t->parms.iph.frag_off = p->iph.frag_off;
	if (t->parms.link != p->link || t->fwmark != fwmark) {
		t->parms.link = p->link;
		t->fwmark = fwmark;
		ipip6_tunnel_bind_dev(t->dev);
	}
	dst_cache_reset(&t->dst_cache);
	netdev_state_change(t->dev);
}
