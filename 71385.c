static __poll_t ib_uverbs_async_event_poll(struct file *filp,
					       struct poll_table_struct *wait)
{
	return ib_uverbs_event_poll(filp->private_data, filp, wait);
}
