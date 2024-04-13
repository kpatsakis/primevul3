void ib_uverbs_release_ucq(struct ib_uverbs_file *file,
			  struct ib_uverbs_completion_event_file *ev_file,
			  struct ib_ucq_object *uobj)
{
	struct ib_uverbs_event *evt, *tmp;

	if (ev_file) {
		spin_lock_irq(&ev_file->ev_queue.lock);
		list_for_each_entry_safe(evt, tmp, &uobj->comp_list, obj_list) {
			list_del(&evt->list);
			kfree(evt);
		}
		spin_unlock_irq(&ev_file->ev_queue.lock);

		uverbs_uobject_put(&ev_file->uobj);
	}

	spin_lock_irq(&file->async_file->ev_queue.lock);
	list_for_each_entry_safe(evt, tmp, &uobj->async_list, obj_list) {
		list_del(&evt->list);
		kfree(evt);
	}
	spin_unlock_irq(&file->async_file->ev_queue.lock);
}
