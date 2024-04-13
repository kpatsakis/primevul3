int vma_wants_writenotify(struct vm_area_struct *vma, pgprot_t vm_page_prot)
{
	vm_flags_t vm_flags = vma->vm_flags;
	const struct vm_operations_struct *vm_ops = vma->vm_ops;

	/* If it was private or non-writable, the write bit is already clear */
	if ((vm_flags & (VM_WRITE|VM_SHARED)) != ((VM_WRITE|VM_SHARED)))
		return 0;

	/* The backer wishes to know when pages are first written to? */
	if (vm_ops && (vm_ops->page_mkwrite || vm_ops->pfn_mkwrite))
		return 1;

	/* The open routine did something to the protections that pgprot_modify
	 * won't preserve? */
	if (pgprot_val(vm_page_prot) !=
	    pgprot_val(vm_pgprot_modify(vm_page_prot, vm_flags)))
		return 0;

	/* Do we need to track softdirty? */
	if (IS_ENABLED(CONFIG_MEM_SOFT_DIRTY) && !(vm_flags & VM_SOFTDIRTY))
		return 1;

	/* Specialty mapping? */
	if (vm_flags & VM_PFNMAP)
		return 0;

	/* Can the mapping track the dirty pages? */
	return vma->vm_file && vma->vm_file->f_mapping &&
		mapping_cap_account_dirty(vma->vm_file->f_mapping);
}
