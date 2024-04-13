static ssize_t ucma_get_event(struct ucma_file *file, const char __user *inbuf,
			      int in_len, int out_len)
{
	struct ucma_context *ctx;
	struct rdma_ucm_get_event cmd;
	struct ucma_event *uevent;
	int ret = 0;

	/*
	 * Old 32 bit user space does not send the 4 byte padding in the
	 * reserved field. We don't care, allow it to keep working.
	 */
	if (out_len < sizeof(uevent->resp) - sizeof(uevent->resp.reserved))
		return -ENOSPC;

	if (copy_from_user(&cmd, inbuf, sizeof(cmd)))
		return -EFAULT;

	mutex_lock(&file->mut);
	while (list_empty(&file->event_list)) {
		mutex_unlock(&file->mut);

		if (file->filp->f_flags & O_NONBLOCK)
			return -EAGAIN;

		if (wait_event_interruptible(file->poll_wait,
					     !list_empty(&file->event_list)))
			return -ERESTARTSYS;

		mutex_lock(&file->mut);
	}

	uevent = list_entry(file->event_list.next, struct ucma_event, list);

	if (uevent->resp.event == RDMA_CM_EVENT_CONNECT_REQUEST) {
		ctx = ucma_alloc_ctx(file);
		if (!ctx) {
			ret = -ENOMEM;
			goto done;
		}
		uevent->ctx->backlog++;
		ctx->cm_id = uevent->cm_id;
		ctx->cm_id->context = ctx;
		uevent->resp.id = ctx->id;
	}

	if (copy_to_user(u64_to_user_ptr(cmd.response),
			 &uevent->resp,
			 min_t(size_t, out_len, sizeof(uevent->resp)))) {
		ret = -EFAULT;
		goto done;
	}

	list_del(&uevent->list);
	uevent->ctx->events_reported++;
	if (uevent->mc)
		uevent->mc->events_reported++;
	kfree(uevent);
done:
	mutex_unlock(&file->mut);
	return ret;
}
