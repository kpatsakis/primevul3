reply_schedule_callback(struct request *const req, u32 ttl, u32 err, struct reply *reply)
{
	struct deferred_reply_callback *d = mm_calloc(1, sizeof(*d));

	if (!d) {
		event_warn("%s: Couldn't allocate space for deferred callback.",
		    __func__);
		return;
	}

	ASSERT_LOCKED(req->base);

	d->request_type = req->request_type;
	d->user_callback = req->user_callback;
	d->ttl = ttl;
	d->err = err;
	if (reply) {
		d->have_reply = 1;
		memcpy(&d->reply, reply, sizeof(struct reply));
	}

	if (req->handle) {
		req->handle->pending_cb = 1;
		d->handle = req->handle;
	}

	event_deferred_cb_init_(
	    &d->deferred,
	    event_get_priority(&req->timeout_event),
	    reply_run_callback,
	    req->user_pointer);
	event_deferred_cb_schedule_(
		req->base->event_base,
		&d->deferred);
}
