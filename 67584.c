asmlinkage __visible notrace struct pt_regs *sync_regs(struct pt_regs *eregs)
{
	struct pt_regs *regs = (struct pt_regs *)this_cpu_read(cpu_current_top_of_stack) - 1;
	if (regs != eregs)
		*regs = *eregs;
	return regs;
}
