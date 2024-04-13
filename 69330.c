follow_huge_pmd(struct mm_struct *mm, unsigned long address,
		pmd_t *pmd, int flags)
{
	struct page *page = NULL;
	spinlock_t *ptl;
	pte_t pte;
retry:
	ptl = pmd_lockptr(mm, pmd);
	spin_lock(ptl);
	/*
	 * make sure that the address range covered by this pmd is not
	 * unmapped from other threads.
	 */
	if (!pmd_huge(*pmd))
		goto out;
	pte = huge_ptep_get((pte_t *)pmd);
	if (pte_present(pte)) {
		page = pmd_page(*pmd) + ((address & ~PMD_MASK) >> PAGE_SHIFT);
		if (flags & FOLL_GET)
			get_page(page);
	} else {
		if (is_hugetlb_entry_migration(pte)) {
			spin_unlock(ptl);
			__migration_entry_wait(mm, (pte_t *)pmd, ptl);
			goto retry;
		}
		/*
		 * hwpoisoned entry is treated as no_page_table in
		 * follow_page_mask().
		 */
	}
out:
	spin_unlock(ptl);
	return page;
}
