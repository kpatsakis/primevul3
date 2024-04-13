void ib_uverbs_wq_event_handler(struct ib_event *event, void *context_ptr)
{
	struct ib_uevent_object *uobj = container_of(event->element.wq->uobject,
						  struct ib_uevent_object, uobject);

	ib_uverbs_async_handler(context_ptr, uobj->uobject.user_handle,
				event->event, &uobj->event_list,
				&uobj->events_reported);
}
