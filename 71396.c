static __poll_t ib_uverbs_event_poll(struct ib_uverbs_event_queue *ev_queue,
					 struct file *filp,
					 struct poll_table_struct *wait)
{
	__poll_t pollflags = 0;

	poll_wait(filp, &ev_queue->poll_wait, wait);

	spin_lock_irq(&ev_queue->lock);
	if (!list_empty(&ev_queue->event_list))
		pollflags = EPOLLIN | EPOLLRDNORM;
	spin_unlock_irq(&ev_queue->lock);

	return pollflags;
}
