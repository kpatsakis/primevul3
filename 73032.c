static struct sk_buff *tun_build_skb(struct tun_struct *tun,
				     struct tun_file *tfile,
				     struct iov_iter *from,
				     struct virtio_net_hdr *hdr,
				     int len, int *skb_xdp)
{
	struct page_frag *alloc_frag = &current->task_frag;
	struct sk_buff *skb;
	struct bpf_prog *xdp_prog;
	int buflen = SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
	unsigned int delta = 0;
	char *buf;
	size_t copied;
	bool xdp_xmit = false;
	int err, pad = TUN_RX_PAD;

	rcu_read_lock();
	xdp_prog = rcu_dereference(tun->xdp_prog);
	if (xdp_prog)
		pad += TUN_HEADROOM;
	buflen += SKB_DATA_ALIGN(len + pad);
	rcu_read_unlock();

	if (unlikely(!skb_page_frag_refill(buflen, alloc_frag, GFP_KERNEL)))
		return ERR_PTR(-ENOMEM);

	buf = (char *)page_address(alloc_frag->page) + alloc_frag->offset;
	copied = copy_page_from_iter(alloc_frag->page,
				     alloc_frag->offset + pad,
				     len, from);
	if (copied != len)
		return ERR_PTR(-EFAULT);

	/* There's a small window that XDP may be set after the check
	 * of xdp_prog above, this should be rare and for simplicity
	 * we do XDP on skb in case the headroom is not enough.
	 */
	if (hdr->gso_type || !xdp_prog)
		*skb_xdp = 1;
	else
		*skb_xdp = 0;

	rcu_read_lock();
	xdp_prog = rcu_dereference(tun->xdp_prog);
	if (xdp_prog && !*skb_xdp) {
		struct xdp_buff xdp;
		void *orig_data;
		u32 act;

		xdp.data_hard_start = buf;
		xdp.data = buf + pad;
		xdp.data_end = xdp.data + len;
		orig_data = xdp.data;
		act = bpf_prog_run_xdp(xdp_prog, &xdp);

		switch (act) {
		case XDP_REDIRECT:
			get_page(alloc_frag->page);
			alloc_frag->offset += buflen;
			err = xdp_do_redirect(tun->dev, &xdp, xdp_prog);
			if (err)
				goto err_redirect;
			return NULL;
		case XDP_TX:
			xdp_xmit = true;
			/* fall through */
		case XDP_PASS:
			delta = orig_data - xdp.data;
			break;
		default:
			bpf_warn_invalid_xdp_action(act);
			/* fall through */
		case XDP_ABORTED:
			trace_xdp_exception(tun->dev, xdp_prog, act);
			/* fall through */
		case XDP_DROP:
			goto err_xdp;
		}
	}

	skb = build_skb(buf, buflen);
	if (!skb) {
		rcu_read_unlock();
		return ERR_PTR(-ENOMEM);
	}

	skb_reserve(skb, pad - delta);
	skb_put(skb, len + delta);
	get_page(alloc_frag->page);
	alloc_frag->offset += buflen;

	if (xdp_xmit) {
		skb->dev = tun->dev;
		generic_xdp_tx(skb, xdp_prog);
		rcu_read_lock();
		return NULL;
	}

	rcu_read_unlock();

	return skb;

err_redirect:
	put_page(alloc_frag->page);
err_xdp:
	rcu_read_unlock();
	this_cpu_inc(tun->pcpu_stats->rx_dropped);
	return NULL;
}
