static int ucma_open(struct inode *inode, struct file *filp)
{
	struct ucma_file *file;

	file = kmalloc(sizeof *file, GFP_KERNEL);
	if (!file)
		return -ENOMEM;

	file->close_wq = alloc_ordered_workqueue("ucma_close_id",
						 WQ_MEM_RECLAIM);
	if (!file->close_wq) {
		kfree(file);
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&file->event_list);
	INIT_LIST_HEAD(&file->ctx_list);
	init_waitqueue_head(&file->poll_wait);
	mutex_init(&file->mut);

	filp->private_data = file;
	file->filp = filp;

	return nonseekable_open(inode, filp);
}
