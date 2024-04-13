static int pagemap_hugetlb_range(pte_t *ptep, unsigned long hmask,
				 unsigned long addr, unsigned long end,
				 struct mm_walk *walk)
{
	struct pagemapread *pm = walk->private;
	struct vm_area_struct *vma = walk->vma;
	u64 flags = 0, frame = 0;
	int err = 0;
	pte_t pte;

	if (vma->vm_flags & VM_SOFTDIRTY)
		flags |= PM_SOFT_DIRTY;

	pte = huge_ptep_get(ptep);
	if (pte_present(pte)) {
		struct page *page = pte_page(pte);

		if (!PageAnon(page))
			flags |= PM_FILE;

		if (page_mapcount(page) == 1)
			flags |= PM_MMAP_EXCLUSIVE;

		flags |= PM_PRESENT;
		if (pm->show_pfn)
			frame = pte_pfn(pte) +
				((addr & ~hmask) >> PAGE_SHIFT);
	}

	for (; addr != end; addr += PAGE_SIZE) {
		pagemap_entry_t pme = make_pme(frame, flags);

		err = add_to_pagemap(addr, &pme, pm);
		if (err)
			return err;
		if (pm->show_pfn && (flags & PM_PRESENT))
			frame++;
	}

	cond_resched();

	return err;
}
