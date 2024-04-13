int rdma_user_mmap_page(struct ib_ucontext *ucontext,
			struct vm_area_struct *vma, struct page *page,
			unsigned long size)
{
	struct rdma_umap_priv *priv = rdma_user_mmap_pre(ucontext, vma, size);

	if (IS_ERR(priv))
		return PTR_ERR(priv);

	if (remap_pfn_range(vma, vma->vm_start, page_to_pfn(page), size,
			    vma->vm_page_prot)) {
		kfree(priv);
		return -EAGAIN;
	}

	rdma_umap_priv_init(priv, vma);
	return 0;
}
