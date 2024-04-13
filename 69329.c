follow_huge_pgd(struct mm_struct *mm, unsigned long address, pgd_t *pgd, int flags)
{
	if (flags & FOLL_GET)
		return NULL;

	return pte_page(*(pte_t *)pgd) + ((address & ~PGDIR_MASK) >> PAGE_SHIFT);
}
