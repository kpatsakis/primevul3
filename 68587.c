icmp6_match(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct icmp6hdr *ic;
	struct icmp6hdr _icmph;
	const struct ip6t_icmp *icmpinfo = par->matchinfo;

	/* Must not be a fragment. */
	if (par->fragoff != 0)
		return false;

	ic = skb_header_pointer(skb, par->thoff, sizeof(_icmph), &_icmph);
	if (ic == NULL) {
		/* We've been asked to examine this packet, and we
		 * can't.  Hence, no choice but to drop.
		 */
		par->hotdrop = true;
		return false;
	}

	return icmp6_type_code_match(icmpinfo->type,
				     icmpinfo->code[0],
				     icmpinfo->code[1],
				     ic->icmp6_type, ic->icmp6_code,
				     !!(icmpinfo->invflags&IP6T_ICMP_INV));
}
