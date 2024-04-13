static ssize_t ib_uverbs_async_event_read(struct file *filp, char __user *buf,
					  size_t count, loff_t *pos)
{
	struct ib_uverbs_async_event_file *file = filp->private_data;

	return ib_uverbs_event_read(&file->ev_queue, file->uverbs_file, filp,
				    buf, count, pos,
				    sizeof(struct ib_uverbs_async_event_desc));
}
