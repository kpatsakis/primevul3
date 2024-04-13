static int do_seccomp(struct pt_regs *regs)
{
	if (!test_thread_flag(TIF_SECCOMP))
		return 0;

	/*
	 * The ABI we present to seccomp tracers is that r3 contains
	 * the syscall return value and orig_gpr3 contains the first
	 * syscall parameter. This is different to the ptrace ABI where
	 * both r3 and orig_gpr3 contain the first syscall parameter.
	 */
	regs->gpr[3] = -ENOSYS;

	/*
	 * We use the __ version here because we have already checked
	 * TIF_SECCOMP. If this fails, there is nothing left to do, we
	 * have already loaded -ENOSYS into r3, or seccomp has put
	 * something else in r3 (via SECCOMP_RET_ERRNO/TRACE).
	 */
	if (__secure_computing(NULL))
		return -1;

	/*
	 * The syscall was allowed by seccomp, restore the register
	 * state to what audit expects.
	 * Note that we use orig_gpr3, which means a seccomp tracer can
	 * modify the first syscall parameter (in orig_gpr3) and also
	 * allow the syscall to proceed.
	 */
	regs->gpr[3] = regs->orig_gpr3;

	return 0;
}
