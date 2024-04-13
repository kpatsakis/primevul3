static int ib_uverbs_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct ib_uverbs_file *file = filp->private_data;
	struct ib_ucontext *ucontext;
	int ret = 0;
	int srcu_key;

	srcu_key = srcu_read_lock(&file->device->disassociate_srcu);
	ucontext = ib_uverbs_get_ucontext_file(file);
	if (IS_ERR(ucontext)) {
		ret = PTR_ERR(ucontext);
		goto out;
	}

	ret = ucontext->device->ops.mmap(ucontext, vma);
out:
	srcu_read_unlock(&file->device->disassociate_srcu, srcu_key);
	return ret;
}
