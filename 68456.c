long do_syscall_trace_enter(struct pt_regs *regs)
{
	user_exit();

	/*
	 * The tracer may decide to abort the syscall, if so tracehook
	 * will return !0. Note that the tracer may also just change
	 * regs->gpr[0] to an invalid syscall number, that is handled
	 * below on the exit path.
	 */
	if (test_thread_flag(TIF_SYSCALL_TRACE) &&
	    tracehook_report_syscall_entry(regs))
		goto skip;

	/* Run seccomp after ptrace; allow it to set gpr[3]. */
	if (do_seccomp(regs))
		return -1;

	/* Avoid trace and audit when syscall is invalid. */
	if (regs->gpr[0] >= NR_syscalls)
		goto skip;

	if (unlikely(test_thread_flag(TIF_SYSCALL_TRACEPOINT)))
		trace_sys_enter(regs, regs->gpr[0]);

#ifdef CONFIG_PPC64
	if (!is_32bit_task())
		audit_syscall_entry(regs->gpr[0], regs->gpr[3], regs->gpr[4],
				    regs->gpr[5], regs->gpr[6]);
	else
#endif
		audit_syscall_entry(regs->gpr[0],
				    regs->gpr[3] & 0xffffffff,
				    regs->gpr[4] & 0xffffffff,
				    regs->gpr[5] & 0xffffffff,
				    regs->gpr[6] & 0xffffffff);

	/* Return the possibly modified but valid syscall number */
	return regs->gpr[0];

skip:
	/*
	 * If we are aborting explicitly, or if the syscall number is
	 * now invalid, set the return value to -ENOSYS.
	 */
	regs->gpr[3] = -ENOSYS;
	return -1;
}
