static struct rdma_umap_priv *rdma_user_mmap_pre(struct ib_ucontext *ucontext,
						 struct vm_area_struct *vma,
						 unsigned long size)
{
	struct ib_uverbs_file *ufile = ucontext->ufile;
	struct rdma_umap_priv *priv;

	if (vma->vm_end - vma->vm_start != size)
		return ERR_PTR(-EINVAL);

	/* Driver is using this wrong, must be called by ib_uverbs_mmap */
	if (WARN_ON(!vma->vm_file ||
		    vma->vm_file->private_data != ufile))
		return ERR_PTR(-EINVAL);
	lockdep_assert_held(&ufile->device->disassociate_srcu);

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return ERR_PTR(-ENOMEM);
	return priv;
}
