static ssize_t ucma_destroy_id(struct ucma_file *file, const char __user *inbuf,
			       int in_len, int out_len)
{
	struct rdma_ucm_destroy_id cmd;
	struct rdma_ucm_destroy_id_resp resp;
	struct ucma_context *ctx;
	int ret = 0;

	if (out_len < sizeof(resp))
		return -ENOSPC;

	if (copy_from_user(&cmd, inbuf, sizeof(cmd)))
		return -EFAULT;

	mutex_lock(&mut);
	ctx = _ucma_find_context(cmd.id, file);
	if (!IS_ERR(ctx))
		idr_remove(&ctx_idr, ctx->id);
	mutex_unlock(&mut);

	if (IS_ERR(ctx))
		return PTR_ERR(ctx);

	mutex_lock(&ctx->file->mut);
	ctx->destroying = 1;
	mutex_unlock(&ctx->file->mut);

	flush_workqueue(ctx->file->close_wq);
	/* At this point it's guaranteed that there is no inflight
	 * closing task */
	mutex_lock(&mut);
	if (!ctx->closing) {
		mutex_unlock(&mut);
		ucma_put_ctx(ctx);
		wait_for_completion(&ctx->comp);
		rdma_destroy_id(ctx->cm_id);
	} else {
		mutex_unlock(&mut);
	}

	resp.events_reported = ucma_free_ctx(ctx);
	if (copy_to_user(u64_to_user_ptr(cmd.response),
			 &resp, sizeof(resp)))
		ret = -EFAULT;

	return ret;
}
