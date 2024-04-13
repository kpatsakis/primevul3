static int ib_uverbs_comp_event_close(struct inode *inode, struct file *filp)
{
	struct ib_uobject *uobj = filp->private_data;
	struct ib_uverbs_completion_event_file *file = container_of(
		uobj, struct ib_uverbs_completion_event_file, uobj);
	struct ib_uverbs_event *entry, *tmp;

	spin_lock_irq(&file->ev_queue.lock);
	list_for_each_entry_safe(entry, tmp, &file->ev_queue.event_list, list) {
		if (entry->counter)
			list_del(&entry->obj_list);
		kfree(entry);
	}
	file->ev_queue.is_closed = 1;
	spin_unlock_irq(&file->ev_queue.lock);

	uverbs_close_fd(filp);

	return 0;
}
