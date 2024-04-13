int split_vma(struct mm_struct *mm, struct vm_area_struct *vma,
	      unsigned long addr, int new_below)
{
	if (mm->map_count >= sysctl_max_map_count)
		return -ENOMEM;

	return __split_vma(mm, vma, addr, new_below);
}
