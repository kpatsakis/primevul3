static int ucma_event_handler(struct rdma_cm_id *cm_id,
			      struct rdma_cm_event *event)
{
	struct ucma_event *uevent;
	struct ucma_context *ctx = cm_id->context;
	int ret = 0;

	uevent = kzalloc(sizeof(*uevent), GFP_KERNEL);
	if (!uevent)
		return event->event == RDMA_CM_EVENT_CONNECT_REQUEST;

	mutex_lock(&ctx->file->mut);
	uevent->cm_id = cm_id;
	ucma_set_event_context(ctx, event, uevent);
	uevent->resp.event = event->event;
	uevent->resp.status = event->status;
	if (cm_id->qp_type == IB_QPT_UD)
		ucma_copy_ud_event(cm_id->device, &uevent->resp.param.ud,
				   &event->param.ud);
	else
		ucma_copy_conn_event(&uevent->resp.param.conn,
				     &event->param.conn);

	if (event->event == RDMA_CM_EVENT_CONNECT_REQUEST) {
		if (!ctx->backlog) {
			ret = -ENOMEM;
			kfree(uevent);
			goto out;
		}
		ctx->backlog--;
	} else if (!ctx->uid || ctx->cm_id != cm_id) {
		/*
		 * We ignore events for new connections until userspace has set
		 * their context.  This can only happen if an error occurs on a
		 * new connection before the user accepts it.  This is okay,
		 * since the accept will just fail later. However, we do need
		 * to release the underlying HW resources in case of a device
		 * removal event.
		 */
		if (event->event == RDMA_CM_EVENT_DEVICE_REMOVAL)
			ucma_removal_event_handler(cm_id);

		kfree(uevent);
		goto out;
	}

	list_add_tail(&uevent->list, &ctx->file->event_list);
	wake_up_interruptible(&ctx->file->poll_wait);
	if (event->event == RDMA_CM_EVENT_DEVICE_REMOVAL)
		ucma_removal_event_handler(cm_id);
out:
	mutex_unlock(&ctx->file->mut);
	return ret;
}
