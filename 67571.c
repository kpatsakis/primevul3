do_spurious_interrupt_bug(struct pt_regs *regs, long error_code)
{
	cond_local_irq_enable(regs);
}
