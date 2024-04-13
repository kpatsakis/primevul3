static void smaps_account(struct mem_size_stats *mss, struct page *page,
		bool compound, bool young, bool dirty, bool locked)
{
	int i, nr = compound ? 1 << compound_order(page) : 1;
	unsigned long size = nr * PAGE_SIZE;

	if (PageAnon(page)) {
		mss->anonymous += size;
		if (!PageSwapBacked(page) && !dirty && !PageDirty(page))
			mss->lazyfree += size;
	}

	mss->resident += size;
	/* Accumulate the size in pages that have been accessed. */
	if (young || page_is_young(page) || PageReferenced(page))
		mss->referenced += size;

	/*
	 * page_count(page) == 1 guarantees the page is mapped exactly once.
	 * If any subpage of the compound page mapped with PTE it would elevate
	 * page_count().
	 */
	if (page_count(page) == 1) {
		if (dirty || PageDirty(page))
			mss->private_dirty += size;
		else
			mss->private_clean += size;
		mss->pss += (u64)size << PSS_SHIFT;
		if (locked)
			mss->pss_locked += (u64)size << PSS_SHIFT;
		return;
	}

	for (i = 0; i < nr; i++, page++) {
		int mapcount = page_mapcount(page);
		unsigned long pss = (PAGE_SIZE << PSS_SHIFT);

		if (mapcount >= 2) {
			if (dirty || PageDirty(page))
				mss->shared_dirty += PAGE_SIZE;
			else
				mss->shared_clean += PAGE_SIZE;
			mss->pss += pss / mapcount;
			if (locked)
				mss->pss_locked += pss / mapcount;
		} else {
			if (dirty || PageDirty(page))
				mss->private_dirty += PAGE_SIZE;
			else
				mss->private_clean += PAGE_SIZE;
			mss->pss += pss;
			if (locked)
				mss->pss_locked += pss;
		}
	}
}
