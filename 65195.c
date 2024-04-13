static ssize_t ucma_migrate_id(struct ucma_file *new_file,
			       const char __user *inbuf,
			       int in_len, int out_len)
{
	struct rdma_ucm_migrate_id cmd;
	struct rdma_ucm_migrate_resp resp;
	struct ucma_context *ctx;
	struct fd f;
	struct ucma_file *cur_file;
	int ret = 0;

	if (copy_from_user(&cmd, inbuf, sizeof(cmd)))
		return -EFAULT;

	/* Get current fd to protect against it being closed */
	f = fdget(cmd.fd);
	if (!f.file)
		return -ENOENT;

	/* Validate current fd and prevent destruction of id. */
	ctx = ucma_get_ctx(f.file->private_data, cmd.id);
	if (IS_ERR(ctx)) {
		ret = PTR_ERR(ctx);
		goto file_put;
	}

	cur_file = ctx->file;
	if (cur_file == new_file) {
		resp.events_reported = ctx->events_reported;
		goto response;
	}

	/*
	 * Migrate events between fd's, maintaining order, and avoiding new
	 * events being added before existing events.
	 */
	ucma_lock_files(cur_file, new_file);
	mutex_lock(&mut);

	list_move_tail(&ctx->list, &new_file->ctx_list);
	ucma_move_events(ctx, new_file);
	ctx->file = new_file;
	resp.events_reported = ctx->events_reported;

	mutex_unlock(&mut);
	ucma_unlock_files(cur_file, new_file);

response:
	if (copy_to_user(u64_to_user_ptr(cmd.response),
			 &resp, sizeof(resp)))
		ret = -EFAULT;

	ucma_put_ctx(ctx);
file_put:
	fdput(f);
	return ret;
}
