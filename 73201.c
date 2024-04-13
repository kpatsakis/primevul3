sch_handle_ingress(struct sk_buff *skb, struct packet_type **pt_prev, int *ret,
		   struct net_device *orig_dev)
{
#ifdef CONFIG_NET_CLS_ACT
	struct tcf_proto *cl = rcu_dereference_bh(skb->dev->ingress_cl_list);
	struct tcf_result cl_res;

	/* If there's at least one ingress present somewhere (so
	 * we get here via enabled static key), remaining devices
	 * that are not configured with an ingress qdisc will bail
	 * out here.
	 */
	if (!cl)
		return skb;
	if (*pt_prev) {
		*ret = deliver_skb(skb, *pt_prev, orig_dev);
		*pt_prev = NULL;
	}

	qdisc_skb_cb(skb)->pkt_len = skb->len;
	skb->tc_at_ingress = 1;
	qdisc_bstats_cpu_update(cl->q, skb);

	switch (tcf_classify(skb, cl, &cl_res, false)) {
	case TC_ACT_OK:
	case TC_ACT_RECLASSIFY:
		skb->tc_index = TC_H_MIN(cl_res.classid);
		break;
	case TC_ACT_SHOT:
		qdisc_qstats_cpu_drop(cl->q);
		kfree_skb(skb);
		return NULL;
	case TC_ACT_STOLEN:
	case TC_ACT_QUEUED:
	case TC_ACT_TRAP:
		consume_skb(skb);
		return NULL;
	case TC_ACT_REDIRECT:
		/* skb_mac_header check was done by cls/act_bpf, so
		 * we can safely push the L2 header back before
		 * redirecting to another netdev
		 */
		__skb_push(skb, skb->mac_len);
		skb_do_redirect(skb);
		return NULL;
	default:
		break;
	}
#endif /* CONFIG_NET_CLS_ACT */
	return skb;
}
