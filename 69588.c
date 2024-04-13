static int blk_mq_queue_reinit_notify(struct notifier_block *nb,
				      unsigned long action, void *hcpu)
{
	struct request_queue *q;

	/*
	 * Before new mappings are established, hotadded cpu might already
	 * start handling requests. This doesn't break anything as we map
	 * offline CPUs to first hardware queue. We will re-init the queue
	 * below to get optimal settings.
	 */
	if (action != CPU_DEAD && action != CPU_DEAD_FROZEN &&
	    action != CPU_ONLINE && action != CPU_ONLINE_FROZEN)
		return NOTIFY_OK;

	mutex_lock(&all_q_mutex);

	/*
	 * We need to freeze and reinit all existing queues.  Freezing
	 * involves synchronous wait for an RCU grace period and doing it
	 * one by one may take a long time.  Start freezing all queues in
	 * one swoop and then wait for the completions so that freezing can
	 * take place in parallel.
	 */
	list_for_each_entry(q, &all_q_list, all_q_node)
		blk_mq_freeze_queue_start(q);
	list_for_each_entry(q, &all_q_list, all_q_node) {
		blk_mq_freeze_queue_wait(q);

		/*
		 * timeout handler can't touch hw queue during the
		 * reinitialization
		 */
		del_timer_sync(&q->timeout);
	}

	list_for_each_entry(q, &all_q_list, all_q_node)
		blk_mq_queue_reinit(q);

	list_for_each_entry(q, &all_q_list, all_q_node)
		blk_mq_unfreeze_queue(q);

	mutex_unlock(&all_q_mutex);
	return NOTIFY_OK;
}
