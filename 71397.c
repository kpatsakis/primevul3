static ssize_t ib_uverbs_event_read(struct ib_uverbs_event_queue *ev_queue,
				    struct ib_uverbs_file *uverbs_file,
				    struct file *filp, char __user *buf,
				    size_t count, loff_t *pos,
				    size_t eventsz)
{
	struct ib_uverbs_event *event;
	int ret = 0;

	spin_lock_irq(&ev_queue->lock);

	while (list_empty(&ev_queue->event_list)) {
		spin_unlock_irq(&ev_queue->lock);

		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;

		if (wait_event_interruptible(ev_queue->poll_wait,
					     (!list_empty(&ev_queue->event_list) ||
			/* The barriers built into wait_event_interruptible()
			 * and wake_up() guarentee this will see the null set
			 * without using RCU
			 */
					     !uverbs_file->device->ib_dev)))
			return -ERESTARTSYS;

		/* If device was disassociated and no event exists set an error */
		if (list_empty(&ev_queue->event_list) &&
		    !uverbs_file->device->ib_dev)
			return -EIO;

		spin_lock_irq(&ev_queue->lock);
	}

	event = list_entry(ev_queue->event_list.next, struct ib_uverbs_event, list);

	if (eventsz > count) {
		ret   = -EINVAL;
		event = NULL;
	} else {
		list_del(ev_queue->event_list.next);
		if (event->counter) {
			++(*event->counter);
			list_del(&event->obj_list);
		}
	}

	spin_unlock_irq(&ev_queue->lock);

	if (event) {
		if (copy_to_user(buf, event, eventsz))
			ret = -EFAULT;
		else
			ret = eventsz;
	}

	kfree(event);

	return ret;
}
