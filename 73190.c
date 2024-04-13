static int netif_receive_skb_internal(struct sk_buff *skb)
{
	int ret;

	net_timestamp_check(netdev_tstamp_prequeue, skb);

	if (skb_defer_rx_timestamp(skb))
		return NET_RX_SUCCESS;

	if (static_key_false(&generic_xdp_needed)) {
		int ret;

		preempt_disable();
		rcu_read_lock();
		ret = do_xdp_generic(rcu_dereference(skb->dev->xdp_prog), skb);
		rcu_read_unlock();
		preempt_enable();

		if (ret != XDP_PASS)
			return NET_RX_DROP;
	}

	rcu_read_lock();
#ifdef CONFIG_RPS
	if (static_key_false(&rps_needed)) {
		struct rps_dev_flow voidflow, *rflow = &voidflow;
		int cpu = get_rps_cpu(skb->dev, skb, &rflow);

		if (cpu >= 0) {
			ret = enqueue_to_backlog(skb, cpu, &rflow->last_qtail);
			rcu_read_unlock();
			return ret;
		}
	}
#endif
	ret = __netif_receive_skb(skb);
	rcu_read_unlock();
	return ret;
}
