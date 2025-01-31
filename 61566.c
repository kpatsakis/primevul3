void kiocb_set_cancel_fn(struct kiocb *iocb, kiocb_cancel_fn *cancel)
{
	struct aio_kiocb *req = container_of(iocb, struct aio_kiocb, common);
	struct kioctx *ctx = req->ki_ctx;
	unsigned long flags;

	spin_lock_irqsave(&ctx->ctx_lock, flags);

	if (!req->ki_list.next)
		list_add(&req->ki_list, &ctx->active_reqs);

	req->ki_cancel = cancel;

	spin_unlock_irqrestore(&ctx->ctx_lock, flags);
}
