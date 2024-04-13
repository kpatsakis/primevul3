static void smaps_pte_entry(pte_t *pte, unsigned long addr,
		struct mm_walk *walk)
{
	struct mem_size_stats *mss = walk->private;
	struct vm_area_struct *vma = walk->vma;
	bool locked = !!(vma->vm_flags & VM_LOCKED);
	struct page *page = NULL;

	if (pte_present(*pte)) {
		page = vm_normal_page(vma, addr, *pte);
	} else if (is_swap_pte(*pte)) {
		swp_entry_t swpent = pte_to_swp_entry(*pte);

		if (!non_swap_entry(swpent)) {
			int mapcount;

			mss->swap += PAGE_SIZE;
			mapcount = swp_swapcount(swpent);
			if (mapcount >= 2) {
				u64 pss_delta = (u64)PAGE_SIZE << PSS_SHIFT;

				do_div(pss_delta, mapcount);
				mss->swap_pss += pss_delta;
			} else {
				mss->swap_pss += (u64)PAGE_SIZE << PSS_SHIFT;
			}
		} else if (is_migration_entry(swpent))
			page = migration_entry_to_page(swpent);
		else if (is_device_private_entry(swpent))
			page = device_private_entry_to_page(swpent);
	} else if (unlikely(IS_ENABLED(CONFIG_SHMEM) && mss->check_shmem_swap
							&& pte_none(*pte))) {
		page = find_get_entry(vma->vm_file->f_mapping,
						linear_page_index(vma, addr));
		if (!page)
			return;

		if (xa_is_value(page))
			mss->swap += PAGE_SIZE;
		else
			put_page(page);

		return;
	}

	if (!page)
		return;

	smaps_account(mss, page, false, pte_young(*pte), pte_dirty(*pte), locked);
}
