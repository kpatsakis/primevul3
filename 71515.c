static int special_mapping_mremap(struct vm_area_struct *new_vma)
{
	struct vm_special_mapping *sm = new_vma->vm_private_data;

	if (WARN_ON_ONCE(current->mm != new_vma->vm_mm))
		return -EFAULT;

	if (sm->mremap)
		return sm->mremap(sm, new_vma);

	return 0;
}
