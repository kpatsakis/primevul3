static void rdma_umap_priv_init(struct rdma_umap_priv *priv,
				struct vm_area_struct *vma)
{
	struct ib_uverbs_file *ufile = vma->vm_file->private_data;

	priv->vma = vma;
	vma->vm_private_data = priv;
	vma->vm_ops = &rdma_umap_ops;

	mutex_lock(&ufile->umap_lock);
	list_add(&priv->list, &ufile->umaps);
	mutex_unlock(&ufile->umap_lock);
}
