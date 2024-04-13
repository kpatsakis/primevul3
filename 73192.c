static int netif_rx_internal(struct sk_buff *skb)
{
	int ret;

	net_timestamp_check(netdev_tstamp_prequeue, skb);

	trace_netif_rx(skb);

	if (static_key_false(&generic_xdp_needed)) {
		int ret;

		preempt_disable();
		rcu_read_lock();
		ret = do_xdp_generic(rcu_dereference(skb->dev->xdp_prog), skb);
		rcu_read_unlock();
		preempt_enable();

		/* Consider XDP consuming the packet a success from
		 * the netdev point of view we do not want to count
		 * this as an error.
		 */
		if (ret != XDP_PASS)
			return NET_RX_SUCCESS;
	}

#ifdef CONFIG_RPS
	if (static_key_false(&rps_needed)) {
		struct rps_dev_flow voidflow, *rflow = &voidflow;
		int cpu;

		preempt_disable();
		rcu_read_lock();

		cpu = get_rps_cpu(skb->dev, skb, &rflow);
		if (cpu < 0)
			cpu = smp_processor_id();

		ret = enqueue_to_backlog(skb, cpu, &rflow->last_qtail);

		rcu_read_unlock();
		preempt_enable();
	} else
#endif
	{
		unsigned int qtail;

		ret = enqueue_to_backlog(skb, get_cpu(), &qtail);
		put_cpu();
	}
	return ret;
}
