int expand_stack(struct vm_area_struct *vma, unsigned long address)
{
	return expand_upwards(vma, address);
}
