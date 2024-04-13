static int ipip6_rcv(struct sk_buff *skb)
{
	const struct iphdr *iph = ip_hdr(skb);
	struct ip_tunnel *tunnel;
	int sifindex;
	int err;

	sifindex = netif_is_l3_master(skb->dev) ? IPCB(skb)->iif : 0;
	tunnel = ipip6_tunnel_lookup(dev_net(skb->dev), skb->dev,
				     iph->saddr, iph->daddr, sifindex);
	if (tunnel) {
		struct pcpu_sw_netstats *tstats;

		if (tunnel->parms.iph.protocol != IPPROTO_IPV6 &&
		    tunnel->parms.iph.protocol != 0)
			goto out;

		skb->mac_header = skb->network_header;
		skb_reset_network_header(skb);
		IPCB(skb)->flags = 0;
		skb->dev = tunnel->dev;

		if (packet_is_spoofed(skb, iph, tunnel)) {
			tunnel->dev->stats.rx_errors++;
			goto out;
		}

		if (iptunnel_pull_header(skb, 0, htons(ETH_P_IPV6),
		    !net_eq(tunnel->net, dev_net(tunnel->dev))))
			goto out;

		err = IP_ECN_decapsulate(iph, skb);
		if (unlikely(err)) {
			if (log_ecn_error)
				net_info_ratelimited("non-ECT from %pI4 with TOS=%#x\n",
						     &iph->saddr, iph->tos);
			if (err > 1) {
				++tunnel->dev->stats.rx_frame_errors;
				++tunnel->dev->stats.rx_errors;
				goto out;
			}
		}

		tstats = this_cpu_ptr(tunnel->dev->tstats);
		u64_stats_update_begin(&tstats->syncp);
		tstats->rx_packets++;
		tstats->rx_bytes += skb->len;
		u64_stats_update_end(&tstats->syncp);

		netif_rx(skb);

		return 0;
	}

	/* no tunnel matched,  let upstream know, ipsec may handle it */
	return 1;
out:
	kfree_skb(skb);
	return 0;
}
