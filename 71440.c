static int smaps_hugetlb_range(pte_t *pte, unsigned long hmask,
				 unsigned long addr, unsigned long end,
				 struct mm_walk *walk)
{
	struct mem_size_stats *mss = walk->private;
	struct vm_area_struct *vma = walk->vma;
	struct page *page = NULL;

	if (pte_present(*pte)) {
		page = vm_normal_page(vma, addr, *pte);
	} else if (is_swap_pte(*pte)) {
		swp_entry_t swpent = pte_to_swp_entry(*pte);

		if (is_migration_entry(swpent))
			page = migration_entry_to_page(swpent);
		else if (is_device_private_entry(swpent))
			page = device_private_entry_to_page(swpent);
	}
	if (page) {
		int mapcount = page_mapcount(page);

		if (mapcount >= 2)
			mss->shared_hugetlb += huge_page_size(hstate_vma(vma));
		else
			mss->private_hugetlb += huge_page_size(hstate_vma(vma));
	}
	return 0;
}
