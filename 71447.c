SYSCALL_DEFINE1(userfaultfd, int, flags)
{
	struct userfaultfd_ctx *ctx;
	int fd;

	BUG_ON(!current->mm);

	/* Check the UFFD_* constants for consistency.  */
	BUILD_BUG_ON(UFFD_CLOEXEC != O_CLOEXEC);
	BUILD_BUG_ON(UFFD_NONBLOCK != O_NONBLOCK);

	if (flags & ~UFFD_SHARED_FCNTL_FLAGS)
		return -EINVAL;

	ctx = kmem_cache_alloc(userfaultfd_ctx_cachep, GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	refcount_set(&ctx->refcount, 1);
	ctx->flags = flags;
	ctx->features = 0;
	ctx->state = UFFD_STATE_WAIT_API;
	ctx->released = false;
	ctx->mmap_changing = false;
	ctx->mm = current->mm;
	/* prevent the mm struct to be freed */
	mmgrab(ctx->mm);

	fd = anon_inode_getfd("[userfaultfd]", &userfaultfd_fops, ctx,
			      O_RDWR | (flags & UFFD_SHARED_FCNTL_FLAGS));
	if (fd < 0) {
		mmdrop(ctx->mm);
		kmem_cache_free(userfaultfd_ctx_cachep, ctx);
	}
	return fd;
}
