void SPEFloatingPointException(struct pt_regs *regs)
{
	unsigned long spefscr;
	int fpexc_mode;
	int code = 0;

	spefscr = current->thread.spefscr;
	fpexc_mode = current->thread.fpexc_mode;

	/* Hardware does not neccessarily set sticky
	 * underflow/overflow/invalid flags */
	if ((spefscr & SPEFSCR_FOVF) && (fpexc_mode & PR_FP_EXC_OVF)) {
		code = FPE_FLTOVF;
		spefscr |= SPEFSCR_FOVFS;
	}
	else if ((spefscr & SPEFSCR_FUNF) && (fpexc_mode & PR_FP_EXC_UND)) {
		code = FPE_FLTUND;
		spefscr |= SPEFSCR_FUNFS;
	}
	else if ((spefscr & SPEFSCR_FDBZ) && (fpexc_mode & PR_FP_EXC_DIV))
		code = FPE_FLTDIV;
	else if ((spefscr & SPEFSCR_FINV) && (fpexc_mode & PR_FP_EXC_INV)) {
		code = FPE_FLTINV;
		spefscr |= SPEFSCR_FINVS;
	}
	else if ((spefscr & (SPEFSCR_FG | SPEFSCR_FX)) && (fpexc_mode & PR_FP_EXC_RES))
		code = FPE_FLTRES;

	current->thread.spefscr = spefscr;

	_exception(SIGFPE, regs, code, regs->nip);
	return;
}
