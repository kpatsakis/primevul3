static int sit_tunnel_rcv(struct sk_buff *skb, u8 ipproto)
{
	const struct iphdr *iph;
	struct ip_tunnel *tunnel;
	int sifindex;

	sifindex = netif_is_l3_master(skb->dev) ? IPCB(skb)->iif : 0;

	iph = ip_hdr(skb);
	tunnel = ipip6_tunnel_lookup(dev_net(skb->dev), skb->dev,
				     iph->saddr, iph->daddr, sifindex);
	if (tunnel) {
		const struct tnl_ptk_info *tpi;

		if (tunnel->parms.iph.protocol != ipproto &&
		    tunnel->parms.iph.protocol != 0)
			goto drop;

		if (!xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb))
			goto drop;
#if IS_ENABLED(CONFIG_MPLS)
		if (ipproto == IPPROTO_MPLS)
			tpi = &mplsip_tpi;
		else
#endif
			tpi = &ipip_tpi;
		if (iptunnel_pull_header(skb, 0, tpi->proto, false))
			goto drop;
		return ip_tunnel_rcv(tunnel, skb, tpi, NULL, log_ecn_error);
	}

	return 1;

drop:
	kfree_skb(skb);
	return 0;
}
