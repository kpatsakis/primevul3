static ssize_t ucma_connect(struct ucma_file *file, const char __user *inbuf,
			    int in_len, int out_len)
{
	struct rdma_ucm_connect cmd;
	struct rdma_conn_param conn_param;
	struct ucma_context *ctx;
	int ret;

	if (copy_from_user(&cmd, inbuf, sizeof(cmd)))
		return -EFAULT;

	if (!cmd.conn_param.valid)
		return -EINVAL;

	ctx = ucma_get_ctx_dev(file, cmd.id);
	if (IS_ERR(ctx))
		return PTR_ERR(ctx);

	ucma_copy_conn_param(ctx->cm_id, &conn_param, &cmd.conn_param);
	ret = rdma_connect(ctx->cm_id, &conn_param);
	ucma_put_ctx(ctx);
	return ret;
}
