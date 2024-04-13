static void vma_link(struct mm_struct *mm, struct vm_area_struct *vma,
			struct vm_area_struct *prev, struct rb_node **rb_link,
			struct rb_node *rb_parent)
{
	struct address_space *mapping = NULL;

	if (vma->vm_file) {
		mapping = vma->vm_file->f_mapping;
		i_mmap_lock_write(mapping);
	}

	__vma_link(mm, vma, prev, rb_link, rb_parent);
	__vma_link_file(vma);

	if (mapping)
		i_mmap_unlock_write(mapping);

	mm->map_count++;
	validate_mm(mm);
}
