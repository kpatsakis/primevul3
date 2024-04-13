static int anon_vma_compatible(struct vm_area_struct *a, struct vm_area_struct *b)
{
	return a->vm_end == b->vm_start &&
		mpol_equal(vma_policy(a), vma_policy(b)) &&
		a->vm_file == b->vm_file &&
		!((a->vm_flags ^ b->vm_flags) & ~(VM_READ|VM_WRITE|VM_EXEC|VM_SOFTDIRTY)) &&
		b->vm_pgoff == a->vm_pgoff + ((b->vm_start - a->vm_start) >> PAGE_SHIFT);
}
