static void remove_vma_list(struct mm_struct *mm, struct vm_area_struct *vma)
{
	unsigned long nr_accounted = 0;

	/* Update high watermark before we lower total_vm */
	update_hiwater_vm(mm);
	do {
		long nrpages = vma_pages(vma);

		if (vma->vm_flags & VM_ACCOUNT)
			nr_accounted += nrpages;
		vm_stat_account(mm, vma->vm_flags, -nrpages);
		vma = remove_vma(vma);
	} while (vma);
	vm_unacct_memory(nr_accounted);
	validate_mm(mm);
}
