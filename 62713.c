void __kprobes single_step_exception(struct pt_regs *regs)
{
	regs->msr &= ~(MSR_SE | MSR_BE);  /* Turn off 'trace' bits */

	if (notify_die(DIE_SSTEP, "single_step", regs, 5,
					5, SIGTRAP) == NOTIFY_STOP)
		return;
	if (debugger_sstep(regs))
		return;

	_exception(SIGTRAP, regs, TRAP_TRACE, regs->nip);
}
