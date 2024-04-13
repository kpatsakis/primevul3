void alignment_exception(struct pt_regs *regs)
{
	int fixed = 0;

	/* we don't implement logging of alignment exceptions */
	if (!(current->thread.align_ctl & PR_UNALIGN_SIGBUS))
		fixed = fix_alignment(regs);

	if (fixed == 1) {
		regs->nip += 4;	/* skip over emulated instruction */
		emulate_single_step(regs);
		return;
	}

	/* Operand address was bad */
	if (fixed == -EFAULT) {
		if (user_mode(regs))
			_exception(SIGSEGV, regs, SEGV_ACCERR, regs->dar);
		else
			/* Search exception table */
			bad_page_fault(regs, regs->dar, SIGSEGV);
		return;
	}
	_exception(SIGBUS, regs, BUS_ADRALN, regs->dar);
}
