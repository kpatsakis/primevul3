void DebugException(struct pt_regs *regs, unsigned long debug_status)
{
	if (debug_status & DBSR_IC) {	/* instruction completion */
		regs->msr &= ~MSR_DE;
		if (user_mode(regs)) {
			current->thread.dbcr0 &= ~DBCR0_IC;
		} else {
			/* Disable instruction completion */
			mtspr(SPRN_DBCR0, mfspr(SPRN_DBCR0) & ~DBCR0_IC);
			/* Clear the instruction completion event */
			mtspr(SPRN_DBSR, DBSR_IC);
			if (debugger_sstep(regs))
				return;
		}
		_exception(SIGTRAP, regs, TRAP_TRACE, 0);
	}
}
