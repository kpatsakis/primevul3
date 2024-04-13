static netdev_tx_t tun_net_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct tun_struct *tun = netdev_priv(dev);
	int txq = skb->queue_mapping;
	struct tun_file *tfile;
	u32 numqueues = 0;

	rcu_read_lock();
	tfile = rcu_dereference(tun->tfiles[txq]);
	numqueues = ACCESS_ONCE(tun->numqueues);

	/* Drop packet if interface is not attached */
	if (txq >= numqueues)
		goto drop;

#ifdef CONFIG_RPS
	if (numqueues == 1 && static_key_false(&rps_needed)) {
		/* Select queue was not called for the skbuff, so we extract the
		 * RPS hash and save it into the flow_table here.
		 */
		__u32 rxhash;

		rxhash = __skb_get_hash_symmetric(skb);
		if (rxhash) {
			struct tun_flow_entry *e;
			e = tun_flow_find(&tun->flows[tun_hashfn(rxhash)],
					rxhash);
			if (e)
				tun_flow_save_rps_rxhash(e, rxhash);
		}
	}
#endif

	tun_debug(KERN_INFO, tun, "tun_net_xmit %d\n", skb->len);

	BUG_ON(!tfile);

	/* Drop if the filter does not like it.
	 * This is a noop if the filter is disabled.
	 * Filter can be enabled only for the TAP devices. */
	if (!check_filter(&tun->txflt, skb))
		goto drop;

	if (tfile->socket.sk->sk_filter &&
	    sk_filter(tfile->socket.sk, skb))
		goto drop;

	if (unlikely(skb_orphan_frags_rx(skb, GFP_ATOMIC)))
		goto drop;

	skb_tx_timestamp(skb);

	/* Orphan the skb - required as we might hang on to it
	 * for indefinite time.
	 */
	skb_orphan(skb);

	nf_reset(skb);

	if (skb_array_produce(&tfile->tx_array, skb))
		goto drop;

	/* Notify and wake up reader process */
	if (tfile->flags & TUN_FASYNC)
		kill_fasync(&tfile->fasync, SIGIO, POLL_IN);
	tfile->socket.sk->sk_data_ready(tfile->socket.sk);

	rcu_read_unlock();
	return NETDEV_TX_OK;

drop:
	this_cpu_inc(tun->pcpu_stats->tx_dropped);
	skb_tx_error(skb);
	kfree_skb(skb);
	rcu_read_unlock();
	return NET_XMIT_DROP;
}
