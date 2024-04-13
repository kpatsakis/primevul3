static void tun_flow_update(struct tun_struct *tun, u32 rxhash,
			    struct tun_file *tfile)
{
	struct hlist_head *head;
	struct tun_flow_entry *e;
	unsigned long delay = tun->ageing_time;
	u16 queue_index = tfile->queue_index;

	if (!rxhash)
		return;
	else
		head = &tun->flows[tun_hashfn(rxhash)];

	rcu_read_lock();

	/* We may get a very small possibility of OOO during switching, not
	 * worth to optimize.*/
	if (tun->numqueues == 1 || tfile->detached)
		goto unlock;

	e = tun_flow_find(head, rxhash);
	if (likely(e)) {
		/* TODO: keep queueing to old queue until it's empty? */
		e->queue_index = queue_index;
		e->updated = jiffies;
		sock_rps_record_flow_hash(e->rps_rxhash);
	} else {
		spin_lock_bh(&tun->lock);
		if (!tun_flow_find(head, rxhash) &&
		    tun->flow_count < MAX_TAP_FLOWS)
			tun_flow_create(tun, head, rxhash, queue_index);

		if (!timer_pending(&tun->flow_gc_timer))
			mod_timer(&tun->flow_gc_timer,
				  round_jiffies_up(jiffies + delay));
		spin_unlock_bh(&tun->lock);
	}

unlock:
	rcu_read_unlock();
}
