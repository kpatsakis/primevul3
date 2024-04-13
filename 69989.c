static int ipip6_fill_info(struct sk_buff *skb, const struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct ip_tunnel_parm *parm = &tunnel->parms;

	if (nla_put_u32(skb, IFLA_IPTUN_LINK, parm->link) ||
	    nla_put_in_addr(skb, IFLA_IPTUN_LOCAL, parm->iph.saddr) ||
	    nla_put_in_addr(skb, IFLA_IPTUN_REMOTE, parm->iph.daddr) ||
	    nla_put_u8(skb, IFLA_IPTUN_TTL, parm->iph.ttl) ||
	    nla_put_u8(skb, IFLA_IPTUN_TOS, parm->iph.tos) ||
	    nla_put_u8(skb, IFLA_IPTUN_PMTUDISC,
		       !!(parm->iph.frag_off & htons(IP_DF))) ||
	    nla_put_u8(skb, IFLA_IPTUN_PROTO, parm->iph.protocol) ||
	    nla_put_be16(skb, IFLA_IPTUN_FLAGS, parm->i_flags) ||
	    nla_put_u32(skb, IFLA_IPTUN_FWMARK, tunnel->fwmark))
		goto nla_put_failure;

#ifdef CONFIG_IPV6_SIT_6RD
	if (nla_put_in6_addr(skb, IFLA_IPTUN_6RD_PREFIX,
			     &tunnel->ip6rd.prefix) ||
	    nla_put_in_addr(skb, IFLA_IPTUN_6RD_RELAY_PREFIX,
			    tunnel->ip6rd.relay_prefix) ||
	    nla_put_u16(skb, IFLA_IPTUN_6RD_PREFIXLEN,
			tunnel->ip6rd.prefixlen) ||
	    nla_put_u16(skb, IFLA_IPTUN_6RD_RELAY_PREFIXLEN,
			tunnel->ip6rd.relay_prefixlen))
		goto nla_put_failure;
#endif

	if (nla_put_u16(skb, IFLA_IPTUN_ENCAP_TYPE,
			tunnel->encap.type) ||
	    nla_put_be16(skb, IFLA_IPTUN_ENCAP_SPORT,
			tunnel->encap.sport) ||
	    nla_put_be16(skb, IFLA_IPTUN_ENCAP_DPORT,
			tunnel->encap.dport) ||
	    nla_put_u16(skb, IFLA_IPTUN_ENCAP_FLAGS,
			tunnel->encap.flags))
		goto nla_put_failure;

	return 0;

nla_put_failure:
	return -EMSGSIZE;
}
