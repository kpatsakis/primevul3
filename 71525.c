pgprot_t vm_get_page_prot(unsigned long vm_flags)
{
	pgprot_t ret = __pgprot(pgprot_val(protection_map[vm_flags &
				(VM_READ|VM_WRITE|VM_EXEC|VM_SHARED)]) |
			pgprot_val(arch_vm_get_page_prot(vm_flags)));

	return arch_filter_pgprot(ret);
}
