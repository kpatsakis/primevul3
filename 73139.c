int do_xdp_generic(struct bpf_prog *xdp_prog, struct sk_buff *skb)
{
	if (xdp_prog) {
		u32 act = netif_receive_generic_xdp(skb, xdp_prog);
		int err;

		if (act != XDP_PASS) {
			switch (act) {
			case XDP_REDIRECT:
				err = xdp_do_generic_redirect(skb->dev, skb,
							      xdp_prog);
				if (err)
					goto out_redir;
			/* fallthru to submit skb */
			case XDP_TX:
				generic_xdp_tx(skb, xdp_prog);
				break;
			}
			return XDP_DROP;
		}
	}
	return XDP_PASS;
out_redir:
	kfree_skb(skb);
	return XDP_DROP;
}
