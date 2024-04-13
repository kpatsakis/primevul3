void SoftwareEmulation(struct pt_regs *regs)
{
	extern int do_mathemu(struct pt_regs *);
	extern int Soft_emulate_8xx(struct pt_regs *);
	int errcode;

	CHECK_FULL_REGS(regs);

	if (!user_mode(regs)) {
		debugger(regs);
		die("Kernel Mode Software FPU Emulation", regs, SIGFPE);
	}

#ifdef CONFIG_MATH_EMULATION
	errcode = do_mathemu(regs);
#else
	errcode = Soft_emulate_8xx(regs);
#endif
	if (errcode) {
		if (errcode > 0)
			_exception(SIGFPE, regs, 0, 0);
		else if (errcode == -EFAULT)
			_exception(SIGSEGV, regs, 0, 0);
		else
			_exception(SIGILL, regs, ILL_ILLOPC, regs->nip);
	} else
		emulate_single_step(regs);
}
