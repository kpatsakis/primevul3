can_vma_merge_after(struct vm_area_struct *vma, unsigned long vm_flags,
		    struct anon_vma *anon_vma, struct file *file,
		    pgoff_t vm_pgoff,
		    struct vm_userfaultfd_ctx vm_userfaultfd_ctx)
{
	if (is_mergeable_vma(vma, file, vm_flags, vm_userfaultfd_ctx) &&
	    is_mergeable_anon_vma(anon_vma, vma->anon_vma, vma)) {
		pgoff_t vm_pglen;
		vm_pglen = vma_pages(vma);
		if (vma->vm_pgoff + vm_pglen == vm_pgoff)
			return 1;
	}
	return 0;
}
