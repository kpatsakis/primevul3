static unsigned long count_vma_pages_range(struct mm_struct *mm,
		unsigned long addr, unsigned long end)
{
	unsigned long nr_pages = 0;
	struct vm_area_struct *vma;

	/* Find first overlaping mapping */
	vma = find_vma_intersection(mm, addr, end);
	if (!vma)
		return 0;

	nr_pages = (min(end, vma->vm_end) -
		max(addr, vma->vm_start)) >> PAGE_SHIFT;

	/* Iterate over the rest of the overlaps */
	for (vma = vma->vm_next; vma; vma = vma->vm_next) {
		unsigned long overlap_len;

		if (vma->vm_start > end)
			break;

		overlap_len = min(end, vma->vm_end) - vma->vm_start;
		nr_pages += overlap_len >> PAGE_SHIFT;
	}

	return nr_pages;
}
