void vmacache_update(unsigned long addr, struct vm_area_struct *newvma)
{
	if (vmacache_valid_mm(newvma->vm_mm))
		current->vmacache.vmas[VMACACHE_HASH(addr)] = newvma;
}
