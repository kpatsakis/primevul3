static void ib_uverbs_free_hw_resources(struct ib_uverbs_device *uverbs_dev,
					struct ib_device *ib_dev)
{
	struct ib_uverbs_file *file;
	struct ib_uverbs_async_event_file *event_file;
	struct ib_event event;

	/* Pending running commands to terminate */
	uverbs_disassociate_api_pre(uverbs_dev);
	event.event = IB_EVENT_DEVICE_FATAL;
	event.element.port_num = 0;
	event.device = ib_dev;

	mutex_lock(&uverbs_dev->lists_mutex);
	while (!list_empty(&uverbs_dev->uverbs_file_list)) {
		file = list_first_entry(&uverbs_dev->uverbs_file_list,
					struct ib_uverbs_file, list);
		list_del_init(&file->list);
		kref_get(&file->ref);

		/* We must release the mutex before going ahead and calling
		 * uverbs_cleanup_ufile, as it might end up indirectly calling
		 * uverbs_close, for example due to freeing the resources (e.g
		 * mmput).
		 */
		mutex_unlock(&uverbs_dev->lists_mutex);

		ib_uverbs_event_handler(&file->event_handler, &event);
		uverbs_destroy_ufile_hw(file, RDMA_REMOVE_DRIVER_REMOVE);
		kref_put(&file->ref, ib_uverbs_release_file);

		mutex_lock(&uverbs_dev->lists_mutex);
	}

	while (!list_empty(&uverbs_dev->uverbs_events_file_list)) {
		event_file = list_first_entry(&uverbs_dev->
					      uverbs_events_file_list,
					      struct ib_uverbs_async_event_file,
					      list);
		spin_lock_irq(&event_file->ev_queue.lock);
		event_file->ev_queue.is_closed = 1;
		spin_unlock_irq(&event_file->ev_queue.lock);

		list_del(&event_file->list);
		ib_unregister_event_handler(
			&event_file->uverbs_file->event_handler);
		event_file->uverbs_file->event_handler.device =
			NULL;

		wake_up_interruptible(&event_file->ev_queue.poll_wait);
		kill_fasync(&event_file->ev_queue.async_queue, SIGIO, POLL_IN);
	}
	mutex_unlock(&uverbs_dev->lists_mutex);

	uverbs_disassociate_api(uverbs_dev->uapi);
}
