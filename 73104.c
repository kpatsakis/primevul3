static int __dev_queue_xmit(struct sk_buff *skb, void *accel_priv)
{
	struct net_device *dev = skb->dev;
	struct netdev_queue *txq;
	struct Qdisc *q;
	int rc = -ENOMEM;

	skb_reset_mac_header(skb);

	if (unlikely(skb_shinfo(skb)->tx_flags & SKBTX_SCHED_TSTAMP))
		__skb_tstamp_tx(skb, NULL, skb->sk, SCM_TSTAMP_SCHED);

	/* Disable soft irqs for various locks below. Also
	 * stops preemption for RCU.
	 */
	rcu_read_lock_bh();

	skb_update_prio(skb);

	qdisc_pkt_len_init(skb);
#ifdef CONFIG_NET_CLS_ACT
	skb->tc_at_ingress = 0;
# ifdef CONFIG_NET_EGRESS
	if (static_key_false(&egress_needed)) {
		skb = sch_handle_egress(skb, &rc, dev);
		if (!skb)
			goto out;
	}
# endif
#endif
	/* If device/qdisc don't need skb->dst, release it right now while
	 * its hot in this cpu cache.
	 */
	if (dev->priv_flags & IFF_XMIT_DST_RELEASE)
		skb_dst_drop(skb);
	else
		skb_dst_force(skb);

	txq = netdev_pick_tx(dev, skb, accel_priv);
	q = rcu_dereference_bh(txq->qdisc);

	trace_net_dev_queue(skb);
	if (q->enqueue) {
		rc = __dev_xmit_skb(skb, q, dev, txq);
		goto out;
	}

	/* The device has no queue. Common case for software devices:
	 * loopback, all the sorts of tunnels...

	 * Really, it is unlikely that netif_tx_lock protection is necessary
	 * here.  (f.e. loopback and IP tunnels are clean ignoring statistics
	 * counters.)
	 * However, it is possible, that they rely on protection
	 * made by us here.

	 * Check this and shot the lock. It is not prone from deadlocks.
	 *Either shot noqueue qdisc, it is even simpler 8)
	 */
	if (dev->flags & IFF_UP) {
		int cpu = smp_processor_id(); /* ok because BHs are off */

		if (txq->xmit_lock_owner != cpu) {
			if (unlikely(__this_cpu_read(xmit_recursion) >
				     XMIT_RECURSION_LIMIT))
				goto recursion_alert;

			skb = validate_xmit_skb(skb, dev);
			if (!skb)
				goto out;

			HARD_TX_LOCK(dev, txq, cpu);

			if (!netif_xmit_stopped(txq)) {
				__this_cpu_inc(xmit_recursion);
				skb = dev_hard_start_xmit(skb, dev, txq, &rc);
				__this_cpu_dec(xmit_recursion);
				if (dev_xmit_complete(rc)) {
					HARD_TX_UNLOCK(dev, txq);
					goto out;
				}
			}
			HARD_TX_UNLOCK(dev, txq);
			net_crit_ratelimited("Virtual device %s asks to queue packet!\n",
					     dev->name);
		} else {
			/* Recursion is detected! It is possible,
			 * unfortunately
			 */
recursion_alert:
			net_crit_ratelimited("Dead loop on virtual device %s, fix it urgently!\n",
					     dev->name);
		}
	}

	rc = -ENETDOWN;
	rcu_read_unlock_bh();

	atomic_long_inc(&dev->tx_dropped);
	kfree_skb_list(skb);
	return rc;
out:
	rcu_read_unlock_bh();
	return rc;
}
