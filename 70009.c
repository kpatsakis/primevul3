static int ipip_rcv(struct sk_buff *skb)
{
	return sit_tunnel_rcv(skb, IPPROTO_IPIP);
}
