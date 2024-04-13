static int snmp_translate(struct nf_conn *ct, int dir, struct sk_buff *skb)
{
	struct iphdr *iph = ip_hdr(skb);
	struct udphdr *udph = (struct udphdr *)((__be32 *)iph + iph->ihl);
	u16 datalen = ntohs(udph->len) - sizeof(struct udphdr);
	char *data = (unsigned char *)udph + sizeof(struct udphdr);
	struct snmp_ctx ctx;
	int ret;

	if (dir == IP_CT_DIR_ORIGINAL) {
		ctx.from = ct->tuplehash[dir].tuple.src.u3.ip;
		ctx.to = ct->tuplehash[!dir].tuple.dst.u3.ip;
	} else {
		ctx.from = ct->tuplehash[!dir].tuple.src.u3.ip;
		ctx.to = ct->tuplehash[dir].tuple.dst.u3.ip;
	}

	if (ctx.from == ctx.to)
		return NF_ACCEPT;

	ctx.begin = (unsigned char *)udph + sizeof(struct udphdr);
	ctx.check = &udph->check;
	ret = asn1_ber_decoder(&nf_nat_snmp_basic_decoder, &ctx, data, datalen);
	if (ret < 0) {
		nf_ct_helper_log(skb, ct, "parser failed\n");
		return NF_DROP;
	}

	return NF_ACCEPT;
}
