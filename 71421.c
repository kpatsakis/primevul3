static inline void clear_soft_dirty(struct vm_area_struct *vma,
		unsigned long addr, pte_t *pte)
{
	/*
	 * The soft-dirty tracker uses #PF-s to catch writes
	 * to pages, so write-protect the pte as well. See the
	 * Documentation/admin-guide/mm/soft-dirty.rst for full description
	 * of how soft-dirty works.
	 */
	pte_t ptent = *pte;

	if (pte_present(ptent)) {
		pte_t old_pte;

		old_pte = ptep_modify_prot_start(vma, addr, pte);
		ptent = pte_wrprotect(old_pte);
		ptent = pte_clear_soft_dirty(ptent);
		ptep_modify_prot_commit(vma, addr, pte, old_pte, ptent);
	} else if (is_swap_pte(ptent)) {
		ptent = pte_swp_clear_soft_dirty(ptent);
		set_pte_at(vma->vm_mm, addr, pte, ptent);
	}
}
