struct file *ib_uverbs_alloc_async_event_file(struct ib_uverbs_file *uverbs_file,
					      struct ib_device	*ib_dev)
{
	struct ib_uverbs_async_event_file *ev_file;
	struct file *filp;

	ev_file = kzalloc(sizeof(*ev_file), GFP_KERNEL);
	if (!ev_file)
		return ERR_PTR(-ENOMEM);

	ib_uverbs_init_event_queue(&ev_file->ev_queue);
	ev_file->uverbs_file = uverbs_file;
	kref_get(&ev_file->uverbs_file->ref);
	kref_init(&ev_file->ref);
	filp = anon_inode_getfile("[infinibandevent]", &uverbs_async_event_fops,
				  ev_file, O_RDONLY);
	if (IS_ERR(filp))
		goto err_put_refs;

	mutex_lock(&uverbs_file->device->lists_mutex);
	list_add_tail(&ev_file->list,
		      &uverbs_file->device->uverbs_events_file_list);
	mutex_unlock(&uverbs_file->device->lists_mutex);

	WARN_ON(uverbs_file->async_file);
	uverbs_file->async_file = ev_file;
	kref_get(&uverbs_file->async_file->ref);
	INIT_IB_EVENT_HANDLER(&uverbs_file->event_handler,
			      ib_dev,
			      ib_uverbs_event_handler);
	ib_register_event_handler(&uverbs_file->event_handler);
	/* At that point async file stuff was fully set */

	return filp;

err_put_refs:
	kref_put(&ev_file->uverbs_file->ref, ib_uverbs_release_file);
	kref_put(&ev_file->ref, ib_uverbs_release_async_event_file);
	return filp;
}
