static void parse_fpe(struct pt_regs *regs)
{
	int code = 0;
	unsigned long fpscr;

	flush_fp_to_thread(current);

	fpscr = current->thread.fpscr.val;

	/* Invalid operation */
	if ((fpscr & FPSCR_VE) && (fpscr & FPSCR_VX))
		code = FPE_FLTINV;

	/* Overflow */
	else if ((fpscr & FPSCR_OE) && (fpscr & FPSCR_OX))
		code = FPE_FLTOVF;

	/* Underflow */
	else if ((fpscr & FPSCR_UE) && (fpscr & FPSCR_UX))
		code = FPE_FLTUND;

	/* Divide by zero */
	else if ((fpscr & FPSCR_ZE) && (fpscr & FPSCR_ZX))
		code = FPE_FLTDIV;

	/* Inexact result */
	else if ((fpscr & FPSCR_XE) && (fpscr & FPSCR_XX))
		code = FPE_FLTRES;

	_exception(SIGFPE, regs, code, regs->nip);
}
