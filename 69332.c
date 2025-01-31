long follow_hugetlb_page(struct mm_struct *mm, struct vm_area_struct *vma,
			 struct page **pages, struct vm_area_struct **vmas,
			 unsigned long *position, unsigned long *nr_pages,
			 long i, unsigned int flags, int *nonblocking)
{
	unsigned long pfn_offset;
	unsigned long vaddr = *position;
	unsigned long remainder = *nr_pages;
	struct hstate *h = hstate_vma(vma);
	int err = -EFAULT;

	while (vaddr < vma->vm_end && remainder) {
		pte_t *pte;
		spinlock_t *ptl = NULL;
		int absent;
		struct page *page;

		/*
		 * If we have a pending SIGKILL, don't keep faulting pages and
		 * potentially allocating memory.
		 */
		if (unlikely(fatal_signal_pending(current))) {
			remainder = 0;
			break;
		}

		/*
		 * Some archs (sparc64, sh*) have multiple pte_ts to
		 * each hugepage.  We have to make sure we get the
		 * first, for the page indexing below to work.
		 *
		 * Note that page table lock is not held when pte is null.
		 */
		pte = huge_pte_offset(mm, vaddr & huge_page_mask(h),
				      huge_page_size(h));
		if (pte)
			ptl = huge_pte_lock(h, mm, pte);
		absent = !pte || huge_pte_none(huge_ptep_get(pte));

		/*
		 * When coredumping, it suits get_dump_page if we just return
		 * an error where there's an empty slot with no huge pagecache
		 * to back it.  This way, we avoid allocating a hugepage, and
		 * the sparse dumpfile avoids allocating disk blocks, but its
		 * huge holes still show up with zeroes where they need to be.
		 */
		if (absent && (flags & FOLL_DUMP) &&
		    !hugetlbfs_pagecache_present(h, vma, vaddr)) {
			if (pte)
				spin_unlock(ptl);
			remainder = 0;
			break;
		}

		/*
		 * We need call hugetlb_fault for both hugepages under migration
		 * (in which case hugetlb_fault waits for the migration,) and
		 * hwpoisoned hugepages (in which case we need to prevent the
		 * caller from accessing to them.) In order to do this, we use
		 * here is_swap_pte instead of is_hugetlb_entry_migration and
		 * is_hugetlb_entry_hwpoisoned. This is because it simply covers
		 * both cases, and because we can't follow correct pages
		 * directly from any kind of swap entries.
		 */
		if (absent || is_swap_pte(huge_ptep_get(pte)) ||
		    ((flags & FOLL_WRITE) &&
		      !huge_pte_write(huge_ptep_get(pte)))) {
			int ret;
			unsigned int fault_flags = 0;

			if (pte)
				spin_unlock(ptl);
			if (flags & FOLL_WRITE)
				fault_flags |= FAULT_FLAG_WRITE;
			if (nonblocking)
				fault_flags |= FAULT_FLAG_ALLOW_RETRY;
			if (flags & FOLL_NOWAIT)
				fault_flags |= FAULT_FLAG_ALLOW_RETRY |
					FAULT_FLAG_RETRY_NOWAIT;
			if (flags & FOLL_TRIED) {
				VM_WARN_ON_ONCE(fault_flags &
						FAULT_FLAG_ALLOW_RETRY);
				fault_flags |= FAULT_FLAG_TRIED;
			}
			ret = hugetlb_fault(mm, vma, vaddr, fault_flags);
			if (ret & VM_FAULT_ERROR) {
				err = vm_fault_to_errno(ret, flags);
				remainder = 0;
				break;
			}
			if (ret & VM_FAULT_RETRY) {
				if (nonblocking)
					*nonblocking = 0;
				*nr_pages = 0;
				/*
				 * VM_FAULT_RETRY must not return an
				 * error, it will return zero
				 * instead.
				 *
				 * No need to update "position" as the
				 * caller will not check it after
				 * *nr_pages is set to 0.
				 */
				return i;
			}
			continue;
		}

		pfn_offset = (vaddr & ~huge_page_mask(h)) >> PAGE_SHIFT;
		page = pte_page(huge_ptep_get(pte));
same_page:
		if (pages) {
			pages[i] = mem_map_offset(page, pfn_offset);
			get_page(pages[i]);
		}

		if (vmas)
			vmas[i] = vma;

		vaddr += PAGE_SIZE;
		++pfn_offset;
		--remainder;
		++i;
		if (vaddr < vma->vm_end && remainder &&
				pfn_offset < pages_per_huge_page(h)) {
			/*
			 * We use pfn_offset to avoid touching the pageframes
			 * of this compound page.
			 */
			goto same_page;
		}
		spin_unlock(ptl);
	}
	*nr_pages = remainder;
	/*
	 * setting position is actually required only if remainder is
	 * not zero but it's faster not to add a "if (remainder)"
	 * branch.
	 */
	*position = vaddr;

	return i ? i : err;
}
