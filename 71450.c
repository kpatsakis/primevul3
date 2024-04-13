static void userfaultfd_ctx_get(struct userfaultfd_ctx *ctx)
{
	refcount_inc(&ctx->refcount);
}
