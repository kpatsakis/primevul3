int is_valid_bugaddr(unsigned long addr)
{
	unsigned short ud;

	if (addr < TASK_SIZE_MAX)
		return 0;

	if (probe_kernel_address((unsigned short *)addr, ud))
		return 0;

	return ud == INSN_UD0 || ud == INSN_UD2;
}
