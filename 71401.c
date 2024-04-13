void ib_uverbs_init_event_queue(struct ib_uverbs_event_queue *ev_queue)
{
	spin_lock_init(&ev_queue->lock);
	INIT_LIST_HEAD(&ev_queue->event_list);
	init_waitqueue_head(&ev_queue->poll_wait);
	ev_queue->is_closed   = 0;
	ev_queue->async_queue = NULL;
}
