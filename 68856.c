static int hns_nic_maybe_stop_tx(
	struct sk_buff **out_skb, int *bnum, struct hnae_ring *ring)
{
	struct sk_buff *skb = *out_skb;
	struct sk_buff *new_skb = NULL;
	int buf_num;

	/* no. of segments (plus a header) */
	buf_num = skb_shinfo(skb)->nr_frags + 1;

	if (unlikely(buf_num > ring->max_desc_num_per_pkt)) {
		if (ring_space(ring) < 1)
			return -EBUSY;

		new_skb = skb_copy(skb, GFP_ATOMIC);
		if (!new_skb)
			return -ENOMEM;

		dev_kfree_skb_any(skb);
		*out_skb = new_skb;
		buf_num = 1;
	} else if (buf_num > ring_space(ring)) {
		return -EBUSY;
	}

	*bnum = buf_num;
	return 0;
}
