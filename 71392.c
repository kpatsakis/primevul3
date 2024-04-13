void ib_uverbs_comp_handler(struct ib_cq *cq, void *cq_context)
{
	struct ib_uverbs_event_queue   *ev_queue = cq_context;
	struct ib_ucq_object	       *uobj;
	struct ib_uverbs_event	       *entry;
	unsigned long			flags;

	if (!ev_queue)
		return;

	spin_lock_irqsave(&ev_queue->lock, flags);
	if (ev_queue->is_closed) {
		spin_unlock_irqrestore(&ev_queue->lock, flags);
		return;
	}

	entry = kmalloc(sizeof(*entry), GFP_ATOMIC);
	if (!entry) {
		spin_unlock_irqrestore(&ev_queue->lock, flags);
		return;
	}

	uobj = container_of(cq->uobject, struct ib_ucq_object, uobject);

	entry->desc.comp.cq_handle = cq->uobject->user_handle;
	entry->counter		   = &uobj->comp_events_reported;

	list_add_tail(&entry->list, &ev_queue->event_list);
	list_add_tail(&entry->obj_list, &uobj->comp_list);
	spin_unlock_irqrestore(&ev_queue->lock, flags);

	wake_up_interruptible(&ev_queue->poll_wait);
	kill_fasync(&ev_queue->async_queue, SIGIO, POLL_IN);
}
