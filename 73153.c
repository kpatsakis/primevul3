static void napi_reuse_skb(struct napi_struct *napi, struct sk_buff *skb)
{
	if (unlikely(skb->pfmemalloc)) {
		consume_skb(skb);
		return;
	}
	__skb_pull(skb, skb_headlen(skb));
	/* restore the reserve we had after netdev_alloc_skb_ip_align() */
	skb_reserve(skb, NET_SKB_PAD + NET_IP_ALIGN - skb_headroom(skb));
	skb->vlan_tci = 0;
	skb->dev = napi->dev;
	skb->skb_iif = 0;
	skb->encapsulation = 0;
	skb_shinfo(skb)->gso_type = 0;
	skb->truesize = SKB_TRUESIZE(skb_end_offset(skb));
	secpath_reset(skb);

	napi->skb = skb;
}
