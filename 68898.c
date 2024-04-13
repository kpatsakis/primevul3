static void force_swapin_readahead(struct vm_area_struct *vma,
		unsigned long start, unsigned long end)
{
	struct mm_walk walk = {
		.mm = vma->vm_mm,
		.pmd_entry = swapin_walk_pmd_entry,
		.private = vma,
	};

	walk_page_range(start, end, &walk);

	lru_add_drain();	/* Push any new pages onto the LRU now */
}
