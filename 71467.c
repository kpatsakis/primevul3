struct vm_area_struct *_install_special_mapping(
	struct mm_struct *mm,
	unsigned long addr, unsigned long len,
	unsigned long vm_flags, const struct vm_special_mapping *spec)
{
	return __install_special_mapping(mm, addr, len, vm_flags, (void *)spec,
					&special_mapping_vmops);
}
