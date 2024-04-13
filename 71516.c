static const char *special_mapping_name(struct vm_area_struct *vma)
{
	return ((struct vm_special_mapping *)vma->vm_private_data)->name;
}
