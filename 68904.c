static int madvise_free_single_vma(struct vm_area_struct *vma,
			unsigned long start_addr, unsigned long end_addr)
{
	unsigned long start, end;
	struct mm_struct *mm = vma->vm_mm;
	struct mmu_gather tlb;

	/* MADV_FREE works for only anon vma at the moment */
	if (!vma_is_anonymous(vma))
		return -EINVAL;

	start = max(vma->vm_start, start_addr);
	if (start >= vma->vm_end)
		return -EINVAL;
	end = min(vma->vm_end, end_addr);
	if (end <= vma->vm_start)
		return -EINVAL;

	lru_add_drain();
	tlb_gather_mmu(&tlb, mm, start, end);
	update_hiwater_rss(mm);

	mmu_notifier_invalidate_range_start(mm, start, end);
	madvise_free_page_range(&tlb, vma, start, end);
	mmu_notifier_invalidate_range_end(mm, start, end);
	tlb_finish_mmu(&tlb, start, end);

	return 0;
}
