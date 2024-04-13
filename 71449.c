void mremap_userfaultfd_prep(struct vm_area_struct *vma,
			     struct vm_userfaultfd_ctx *vm_ctx)
{
	struct userfaultfd_ctx *ctx;

	ctx = vma->vm_userfaultfd_ctx.ctx;

	if (!ctx)
		return;

	if (ctx->features & UFFD_FEATURE_EVENT_REMAP) {
		vm_ctx->ctx = ctx;
		userfaultfd_ctx_get(ctx);
		WRITE_ONCE(ctx->mmap_changing, true);
	} else {
		/* Drop uffd context if remap feature not enabled */
		vma->vm_userfaultfd_ctx = NULL_VM_UFFD_CTX;
		vma->vm_flags &= ~(VM_UFFD_WP | VM_UFFD_MISSING);
	}
}
