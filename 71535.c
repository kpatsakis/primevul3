static __always_inline void vma_rb_erase(struct vm_area_struct *vma,
					 struct rb_root *root)
{
	/*
	 * All rb_subtree_gap values must be consistent prior to erase,
	 * with the possible exception of the vma being erased.
	 */
	validate_mm_rb(root, vma);

	__vma_rb_erase(vma, root);
}
