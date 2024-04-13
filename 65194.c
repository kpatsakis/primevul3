static ssize_t ucma_leave_multicast(struct ucma_file *file,
				    const char __user *inbuf,
				    int in_len, int out_len)
{
	struct rdma_ucm_destroy_id cmd;
	struct rdma_ucm_destroy_id_resp resp;
	struct ucma_multicast *mc;
	int ret = 0;

	if (out_len < sizeof(resp))
		return -ENOSPC;

	if (copy_from_user(&cmd, inbuf, sizeof(cmd)))
		return -EFAULT;

	mutex_lock(&mut);
	mc = idr_find(&multicast_idr, cmd.id);
	if (!mc)
		mc = ERR_PTR(-ENOENT);
	else if (mc->ctx->file != file)
		mc = ERR_PTR(-EINVAL);
	else if (!atomic_inc_not_zero(&mc->ctx->ref))
		mc = ERR_PTR(-ENXIO);
	else
		idr_remove(&multicast_idr, mc->id);
	mutex_unlock(&mut);

	if (IS_ERR(mc)) {
		ret = PTR_ERR(mc);
		goto out;
	}

	rdma_leave_multicast(mc->ctx->cm_id, (struct sockaddr *) &mc->addr);
	mutex_lock(&mc->ctx->file->mut);
	ucma_cleanup_mc_events(mc);
	list_del(&mc->list);
	mutex_unlock(&mc->ctx->file->mut);

	ucma_put_ctx(mc->ctx);
	resp.events_reported = mc->events_reported;
	kfree(mc);

	if (copy_to_user(u64_to_user_ptr(cmd.response),
			 &resp, sizeof(resp)))
		ret = -EFAULT;
out:
	return ret;
}
