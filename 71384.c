static int ib_uverbs_async_event_fasync(int fd, struct file *filp, int on)
{
	struct ib_uverbs_event_queue *ev_queue = filp->private_data;

	return fasync_helper(fd, filp, on, &ev_queue->async_queue);
}
