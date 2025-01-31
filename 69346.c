pte_t *huge_pte_offset(struct mm_struct *mm,
		       unsigned long addr, unsigned long sz)
{
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;

	pgd = pgd_offset(mm, addr);
	if (!pgd_present(*pgd))
		return NULL;
	p4d = p4d_offset(pgd, addr);
	if (!p4d_present(*p4d))
		return NULL;

	pud = pud_offset(p4d, addr);
	if (sz != PUD_SIZE && pud_none(*pud))
		return NULL;
	/* hugepage or swap? */
	if (pud_huge(*pud) || !pud_present(*pud))
		return (pte_t *)pud;

	pmd = pmd_offset(pud, addr);
	if (sz != PMD_SIZE && pmd_none(*pmd))
		return NULL;
	/* hugepage or swap? */
	if (pmd_huge(*pmd) || !pmd_present(*pmd))
		return (pte_t *)pmd;

	return NULL;
}
