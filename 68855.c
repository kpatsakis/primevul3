static int hns_nic_maybe_stop_tso(
	struct sk_buff **out_skb, int *bnum, struct hnae_ring *ring)
{
	int i;
	int size;
	int buf_num;
	int frag_num;
	struct sk_buff *skb = *out_skb;
	struct sk_buff *new_skb = NULL;
	struct skb_frag_struct *frag;

	size = skb_headlen(skb);
	buf_num = (size + BD_MAX_SEND_SIZE - 1) / BD_MAX_SEND_SIZE;

	frag_num = skb_shinfo(skb)->nr_frags;
	for (i = 0; i < frag_num; i++) {
		frag = &skb_shinfo(skb)->frags[i];
		size = skb_frag_size(frag);
		buf_num += (size + BD_MAX_SEND_SIZE - 1) / BD_MAX_SEND_SIZE;
	}

	if (unlikely(buf_num > ring->max_desc_num_per_pkt)) {
		buf_num = (skb->len + BD_MAX_SEND_SIZE - 1) / BD_MAX_SEND_SIZE;
		if (ring_space(ring) < buf_num)
			return -EBUSY;
		/* manual split the send packet */
		new_skb = skb_copy(skb, GFP_ATOMIC);
		if (!new_skb)
			return -ENOMEM;
		dev_kfree_skb_any(skb);
		*out_skb = new_skb;

	} else if (ring_space(ring) < buf_num) {
		return -EBUSY;
	}

	*bnum = buf_num;
	return 0;
}
