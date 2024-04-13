int vm_brk(unsigned long addr, unsigned long len)
{
	return vm_brk_flags(addr, len, 0);
}
