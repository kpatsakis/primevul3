static void unmap_region(struct mm_struct *mm,
		struct vm_area_struct *vma, struct vm_area_struct *prev,
		unsigned long start, unsigned long end)
{
	struct vm_area_struct *next = prev ? prev->vm_next : mm->mmap;
	struct mmu_gather tlb;

	lru_add_drain();
	tlb_gather_mmu(&tlb, mm, start, end);
	update_hiwater_rss(mm);
	unmap_vmas(&tlb, vma, start, end);
	free_pgtables(&tlb, vma, prev ? prev->vm_end : FIRST_USER_ADDRESS,
				 next ? next->vm_start : USER_PGTABLES_CEILING);
	tlb_finish_mmu(&tlb, start, end);
}
