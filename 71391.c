static ssize_t ib_uverbs_comp_event_read(struct file *filp, char __user *buf,
					 size_t count, loff_t *pos)
{
	struct ib_uverbs_completion_event_file *comp_ev_file =
		filp->private_data;

	return ib_uverbs_event_read(&comp_ev_file->ev_queue,
				    comp_ev_file->uobj.ufile, filp,
				    buf, count, pos,
				    sizeof(struct ib_uverbs_comp_event_desc));
}
