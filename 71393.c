void ib_uverbs_cq_event_handler(struct ib_event *event, void *context_ptr)
{
	struct ib_ucq_object *uobj = container_of(event->element.cq->uobject,
						  struct ib_ucq_object, uobject);

	ib_uverbs_async_handler(uobj->uobject.ufile, uobj->uobject.user_handle,
				event->event, &uobj->async_list,
				&uobj->async_events_reported);
}
