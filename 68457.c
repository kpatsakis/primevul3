void do_syscall_trace_leave(struct pt_regs *regs)
{
	int step;

	audit_syscall_exit(regs);

	if (unlikely(test_thread_flag(TIF_SYSCALL_TRACEPOINT)))
		trace_sys_exit(regs, regs->result);

	step = test_thread_flag(TIF_SINGLESTEP);
	if (step || test_thread_flag(TIF_SYSCALL_TRACE))
		tracehook_report_syscall_exit(regs, step);

	user_enter();
}
