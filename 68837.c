static struct sk_buff *hns_assemble_skb(struct net_device *ndev)
{
	struct sk_buff *skb;
	struct ethhdr *ethhdr;
	int frame_len;

	/* allocate test skb */
	skb = alloc_skb(64, GFP_KERNEL);
	if (!skb)
		return NULL;

	skb_put(skb, 64);
	skb->dev = ndev;
	memset(skb->data, 0xFF, skb->len);

	/* must be tcp/ip package */
	ethhdr = (struct ethhdr *)skb->data;
	ethhdr->h_proto = htons(ETH_P_IP);

	frame_len = skb->len & (~1ul);
	memset(&skb->data[frame_len / 2], 0xAA,
	       frame_len / 2 - 1);

	skb->queue_mapping = HNS_LB_TX_RING;

	return skb;
}
