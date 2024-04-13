static netdev_tx_t ipip6_tunnel_xmit(struct sk_buff *skb,
				     struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	const struct iphdr  *tiph = &tunnel->parms.iph;
	const struct ipv6hdr *iph6 = ipv6_hdr(skb);
	u8     tos = tunnel->parms.iph.tos;
	__be16 df = tiph->frag_off;
	struct rtable *rt;		/* Route to the other host */
	struct net_device *tdev;	/* Device to other host */
	unsigned int max_headroom;	/* The extra header space needed */
	__be32 dst = tiph->daddr;
	struct flowi4 fl4;
	int    mtu;
	const struct in6_addr *addr6;
	int addr_type;
	u8 ttl;
	u8 protocol = IPPROTO_IPV6;
	int t_hlen = tunnel->hlen + sizeof(struct iphdr);

	if (tos == 1)
		tos = ipv6_get_dsfield(iph6);

	/* ISATAP (RFC4214) - must come before 6to4 */
	if (dev->priv_flags & IFF_ISATAP) {
		struct neighbour *neigh = NULL;
		bool do_tx_error = false;

		if (skb_dst(skb))
			neigh = dst_neigh_lookup(skb_dst(skb), &iph6->daddr);

		if (!neigh) {
			net_dbg_ratelimited("nexthop == NULL\n");
			goto tx_error;
		}

		addr6 = (const struct in6_addr *)&neigh->primary_key;
		addr_type = ipv6_addr_type(addr6);

		if ((addr_type & IPV6_ADDR_UNICAST) &&
		     ipv6_addr_is_isatap(addr6))
			dst = addr6->s6_addr32[3];
		else
			do_tx_error = true;

		neigh_release(neigh);
		if (do_tx_error)
			goto tx_error;
	}

	if (!dst)
		dst = try_6rd(tunnel, &iph6->daddr);

	if (!dst) {
		struct neighbour *neigh = NULL;
		bool do_tx_error = false;

		if (skb_dst(skb))
			neigh = dst_neigh_lookup(skb_dst(skb), &iph6->daddr);

		if (!neigh) {
			net_dbg_ratelimited("nexthop == NULL\n");
			goto tx_error;
		}

		addr6 = (const struct in6_addr *)&neigh->primary_key;
		addr_type = ipv6_addr_type(addr6);

		if (addr_type == IPV6_ADDR_ANY) {
			addr6 = &ipv6_hdr(skb)->daddr;
			addr_type = ipv6_addr_type(addr6);
		}

		if ((addr_type & IPV6_ADDR_COMPATv4) != 0)
			dst = addr6->s6_addr32[3];
		else
			do_tx_error = true;

		neigh_release(neigh);
		if (do_tx_error)
			goto tx_error;
	}

	flowi4_init_output(&fl4, tunnel->parms.link, tunnel->fwmark,
			   RT_TOS(tos), RT_SCOPE_UNIVERSE, IPPROTO_IPV6,
			   0, dst, tiph->saddr, 0, 0,
			   sock_net_uid(tunnel->net, NULL));
	rt = ip_route_output_flow(tunnel->net, &fl4, NULL);

	if (IS_ERR(rt)) {
		dev->stats.tx_carrier_errors++;
		goto tx_error_icmp;
	}
	if (rt->rt_type != RTN_UNICAST) {
		ip_rt_put(rt);
		dev->stats.tx_carrier_errors++;
		goto tx_error_icmp;
	}
	tdev = rt->dst.dev;

	if (tdev == dev) {
		ip_rt_put(rt);
		dev->stats.collisions++;
		goto tx_error;
	}

	if (iptunnel_handle_offloads(skb, SKB_GSO_IPXIP4)) {
		ip_rt_put(rt);
		goto tx_error;
	}

	if (df) {
		mtu = dst_mtu(&rt->dst) - t_hlen;

		if (mtu < 68) {
			dev->stats.collisions++;
			ip_rt_put(rt);
			goto tx_error;
		}

		if (mtu < IPV6_MIN_MTU) {
			mtu = IPV6_MIN_MTU;
			df = 0;
		}

		if (tunnel->parms.iph.daddr)
			skb_dst_update_pmtu(skb, mtu);

		if (skb->len > mtu && !skb_is_gso(skb)) {
			icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu);
			ip_rt_put(rt);
			goto tx_error;
		}
	}

	if (tunnel->err_count > 0) {
		if (time_before(jiffies,
				tunnel->err_time + IPTUNNEL_ERR_TIMEO)) {
			tunnel->err_count--;
			dst_link_failure(skb);
		} else
			tunnel->err_count = 0;
	}

	/*
	 * Okay, now see if we can stuff it in the buffer as-is.
	 */
	max_headroom = LL_RESERVED_SPACE(tdev) + t_hlen;

	if (skb_headroom(skb) < max_headroom || skb_shared(skb) ||
	    (skb_cloned(skb) && !skb_clone_writable(skb, 0))) {
		struct sk_buff *new_skb = skb_realloc_headroom(skb, max_headroom);
		if (!new_skb) {
			ip_rt_put(rt);
			dev->stats.tx_dropped++;
			kfree_skb(skb);
			return NETDEV_TX_OK;
		}
		if (skb->sk)
			skb_set_owner_w(new_skb, skb->sk);
		dev_kfree_skb(skb);
		skb = new_skb;
		iph6 = ipv6_hdr(skb);
	}
	ttl = tiph->ttl;
	if (ttl == 0)
		ttl = iph6->hop_limit;
	tos = INET_ECN_encapsulate(tos, ipv6_get_dsfield(iph6));

	if (ip_tunnel_encap(skb, tunnel, &protocol, &fl4) < 0) {
		ip_rt_put(rt);
		goto tx_error;
	}

	skb_set_inner_ipproto(skb, IPPROTO_IPV6);

	iptunnel_xmit(NULL, rt, skb, fl4.saddr, fl4.daddr, protocol, tos, ttl,
		      df, !net_eq(tunnel->net, dev_net(dev)));
	return NETDEV_TX_OK;

tx_error_icmp:
	dst_link_failure(skb);
tx_error:
	kfree_skb(skb);
	dev->stats.tx_errors++;
	return NETDEV_TX_OK;
}
