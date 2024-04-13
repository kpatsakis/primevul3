static u32 netif_receive_generic_xdp(struct sk_buff *skb,
				     struct bpf_prog *xdp_prog)
{
	struct xdp_buff xdp;
	u32 act = XDP_DROP;
	void *orig_data;
	int hlen, off;
	u32 mac_len;

	/* Reinjected packets coming from act_mirred or similar should
	 * not get XDP generic processing.
	 */
	if (skb_cloned(skb))
		return XDP_PASS;

	if (skb_linearize(skb))
		goto do_drop;

	/* The XDP program wants to see the packet starting at the MAC
	 * header.
	 */
	mac_len = skb->data - skb_mac_header(skb);
	hlen = skb_headlen(skb) + mac_len;
	xdp.data = skb->data - mac_len;
	xdp.data_end = xdp.data + hlen;
	xdp.data_hard_start = skb->data - skb_headroom(skb);
	orig_data = xdp.data;

	act = bpf_prog_run_xdp(xdp_prog, &xdp);

	off = xdp.data - orig_data;
	if (off > 0)
		__skb_pull(skb, off);
	else if (off < 0)
		__skb_push(skb, -off);
	skb->mac_header += off;

	switch (act) {
	case XDP_REDIRECT:
	case XDP_TX:
		__skb_push(skb, mac_len);
		/* fall through */
	case XDP_PASS:
		break;

	default:
		bpf_warn_invalid_xdp_action(act);
		/* fall through */
	case XDP_ABORTED:
		trace_xdp_exception(skb->dev, xdp_prog, act);
		/* fall through */
	case XDP_DROP:
	do_drop:
		kfree_skb(skb);
		break;
	}

	return act;
}
