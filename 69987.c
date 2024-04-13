static int ipip6_err(struct sk_buff *skb, u32 info)
{
	const struct iphdr *iph = (const struct iphdr *)skb->data;
	const int type = icmp_hdr(skb)->type;
	const int code = icmp_hdr(skb)->code;
	unsigned int data_len = 0;
	struct ip_tunnel *t;
	int sifindex;
	int err;

	switch (type) {
	default:
	case ICMP_PARAMETERPROB:
		return 0;

	case ICMP_DEST_UNREACH:
		switch (code) {
		case ICMP_SR_FAILED:
			/* Impossible event. */
			return 0;
		default:
			/* All others are translated to HOST_UNREACH.
			   rfc2003 contains "deep thoughts" about NET_UNREACH,
			   I believe they are just ether pollution. --ANK
			 */
			break;
		}
		break;
	case ICMP_TIME_EXCEEDED:
		if (code != ICMP_EXC_TTL)
			return 0;
		data_len = icmp_hdr(skb)->un.reserved[1] * 4; /* RFC 4884 4.1 */
		break;
	case ICMP_REDIRECT:
		break;
	}

	err = -ENOENT;

	sifindex = netif_is_l3_master(skb->dev) ? IPCB(skb)->iif : 0;
	t = ipip6_tunnel_lookup(dev_net(skb->dev), skb->dev,
				iph->daddr, iph->saddr, sifindex);
	if (!t)
		goto out;

	if (type == ICMP_DEST_UNREACH && code == ICMP_FRAG_NEEDED) {
		ipv4_update_pmtu(skb, dev_net(skb->dev), info,
				 t->parms.link, iph->protocol);
		err = 0;
		goto out;
	}
	if (type == ICMP_REDIRECT) {
		ipv4_redirect(skb, dev_net(skb->dev), t->parms.link,
			      iph->protocol);
		err = 0;
		goto out;
	}

	err = 0;
	if (__in6_dev_get(skb->dev) &&
	    !ip6_err_gen_icmpv6_unreach(skb, iph->ihl * 4, type, data_len))
		goto out;

	if (t->parms.iph.daddr == 0)
		goto out;

	if (t->parms.iph.ttl == 0 && type == ICMP_TIME_EXCEEDED)
		goto out;

	if (time_before(jiffies, t->err_time + IPTUNNEL_ERR_TIMEO))
		t->err_count++;
	else
		t->err_count = 1;
	t->err_time = jiffies;
out:
	return err;
}
