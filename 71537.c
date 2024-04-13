static inline void vma_rb_insert(struct vm_area_struct *vma,
				 struct rb_root *root)
{
	/* All rb_subtree_gap values must be consistent prior to insertion */
	validate_mm_rb(root, NULL);

	rb_insert_augmented(&vma->vm_rb, root, &vma_gap_callbacks);
}
