tcpmss_tg6(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct ipv6hdr *ipv6h = ipv6_hdr(skb);
	u8 nexthdr;
	__be16 frag_off, oldlen, newlen;
	int tcphoff;
	int ret;

	nexthdr = ipv6h->nexthdr;
	tcphoff = ipv6_skip_exthdr(skb, sizeof(*ipv6h), &nexthdr, &frag_off);
	if (tcphoff < 0)
		return NF_DROP;
	ret = tcpmss_mangle_packet(skb, par,
				   PF_INET6,
				   tcphoff,
				   sizeof(*ipv6h) + sizeof(struct tcphdr));
	if (ret < 0)
		return NF_DROP;
	if (ret > 0) {
		ipv6h = ipv6_hdr(skb);
		oldlen = ipv6h->payload_len;
		newlen = htons(ntohs(oldlen) + ret);
		if (skb->ip_summed == CHECKSUM_COMPLETE)
			skb->csum = csum_add(csum_sub(skb->csum, oldlen),
					     newlen);
		ipv6h->payload_len = newlen;
	}
	return XT_CONTINUE;
}
