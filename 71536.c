static __always_inline void vma_rb_erase_ignore(struct vm_area_struct *vma,
						struct rb_root *root,
						struct vm_area_struct *ignore)
{
	/*
	 * All rb_subtree_gap values must be consistent prior to erase,
	 * with the possible exception of the "next" vma being erased if
	 * next->vm_start was reduced.
	 */
	validate_mm_rb(root, ignore);

	__vma_rb_erase(vma, root);
}
