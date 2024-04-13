static void tun_flow_cleanup(unsigned long data)
{
	struct tun_struct *tun = (struct tun_struct *)data;
	unsigned long delay = tun->ageing_time;
	unsigned long next_timer = jiffies + delay;
	unsigned long count = 0;
	int i;

	tun_debug(KERN_INFO, tun, "tun_flow_cleanup\n");

	spin_lock_bh(&tun->lock);
	for (i = 0; i < TUN_NUM_FLOW_ENTRIES; i++) {
		struct tun_flow_entry *e;
		struct hlist_node *n;

		hlist_for_each_entry_safe(e, n, &tun->flows[i], hash_link) {
			unsigned long this_timer;
			count++;
			this_timer = e->updated + delay;
			if (time_before_eq(this_timer, jiffies))
				tun_flow_delete(tun, e);
			else if (time_before(this_timer, next_timer))
				next_timer = this_timer;
		}
	}

	if (count)
		mod_timer(&tun->flow_gc_timer, round_jiffies_up(next_timer));
	spin_unlock_bh(&tun->lock);
}
