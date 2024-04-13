void instruction_breakpoint_exception(struct pt_regs *regs)
{
	if (notify_die(DIE_IABR_MATCH, "iabr_match", regs, 5,
					5, SIGTRAP) == NOTIFY_STOP)
		return;
	if (debugger_iabr_match(regs))
		return;
	_exception(SIGTRAP, regs, TRAP_BRKPT, regs->nip);
}
