static inline void cond_local_irq_enable(struct pt_regs *regs)
{
	if (regs->flags & X86_EFLAGS_IF)
		local_irq_enable();
}
