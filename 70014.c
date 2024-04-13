static bool packet_is_spoofed(struct sk_buff *skb,
			      const struct iphdr *iph,
			      struct ip_tunnel *tunnel)
{
	const struct ipv6hdr *ipv6h;

	if (tunnel->dev->priv_flags & IFF_ISATAP) {
		if (!isatap_chksrc(skb, iph, tunnel))
			return true;

		return false;
	}

	if (tunnel->dev->flags & IFF_POINTOPOINT)
		return false;

	ipv6h = ipv6_hdr(skb);

	if (unlikely(is_spoofed_6rd(tunnel, iph->saddr, &ipv6h->saddr))) {
		net_warn_ratelimited("Src spoofed %pI4/%pI6c -> %pI4/%pI6c\n",
				     &iph->saddr, &ipv6h->saddr,
				     &iph->daddr, &ipv6h->daddr);
		return true;
	}

	if (likely(!is_spoofed_6rd(tunnel, iph->daddr, &ipv6h->daddr)))
		return false;

	if (only_dnatted(tunnel, &ipv6h->daddr))
		return false;

	net_warn_ratelimited("Dst spoofed %pI4/%pI6c -> %pI4/%pI6c\n",
			     &iph->saddr, &ipv6h->saddr,
			     &iph->daddr, &ipv6h->daddr);
	return true;
}
