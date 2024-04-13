static struct tun_flow_entry *tun_flow_create(struct tun_struct *tun,
					      struct hlist_head *head,
					      u32 rxhash, u16 queue_index)
{
	struct tun_flow_entry *e = kmalloc(sizeof(*e), GFP_ATOMIC);

	if (e) {
		tun_debug(KERN_INFO, tun, "create flow: hash %u index %u\n",
			  rxhash, queue_index);
		e->updated = jiffies;
		e->rxhash = rxhash;
		e->rps_rxhash = 0;
		e->queue_index = queue_index;
		e->tun = tun;
		hlist_add_head_rcu(&e->hash_link, head);
		++tun->flow_count;
	}
	return e;
}
