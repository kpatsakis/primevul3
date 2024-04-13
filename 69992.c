static void ipip6_netlink_parms(struct nlattr *data[],
				struct ip_tunnel_parm *parms,
				__u32 *fwmark)
{
	memset(parms, 0, sizeof(*parms));

	parms->iph.version = 4;
	parms->iph.protocol = IPPROTO_IPV6;
	parms->iph.ihl = 5;
	parms->iph.ttl = 64;

	if (!data)
		return;

	if (data[IFLA_IPTUN_LINK])
		parms->link = nla_get_u32(data[IFLA_IPTUN_LINK]);

	if (data[IFLA_IPTUN_LOCAL])
		parms->iph.saddr = nla_get_be32(data[IFLA_IPTUN_LOCAL]);

	if (data[IFLA_IPTUN_REMOTE])
		parms->iph.daddr = nla_get_be32(data[IFLA_IPTUN_REMOTE]);

	if (data[IFLA_IPTUN_TTL]) {
		parms->iph.ttl = nla_get_u8(data[IFLA_IPTUN_TTL]);
		if (parms->iph.ttl)
			parms->iph.frag_off = htons(IP_DF);
	}

	if (data[IFLA_IPTUN_TOS])
		parms->iph.tos = nla_get_u8(data[IFLA_IPTUN_TOS]);

	if (!data[IFLA_IPTUN_PMTUDISC] || nla_get_u8(data[IFLA_IPTUN_PMTUDISC]))
		parms->iph.frag_off = htons(IP_DF);

	if (data[IFLA_IPTUN_FLAGS])
		parms->i_flags = nla_get_be16(data[IFLA_IPTUN_FLAGS]);

	if (data[IFLA_IPTUN_PROTO])
		parms->iph.protocol = nla_get_u8(data[IFLA_IPTUN_PROTO]);

	if (data[IFLA_IPTUN_FWMARK])
		*fwmark = nla_get_u32(data[IFLA_IPTUN_FWMARK]);
}
