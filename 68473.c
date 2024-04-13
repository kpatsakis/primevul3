static long ppc_del_hwdebug(struct task_struct *child, long data)
{
#ifdef CONFIG_HAVE_HW_BREAKPOINT
	int ret = 0;
	struct thread_struct *thread = &(child->thread);
	struct perf_event *bp;
#endif /* CONFIG_HAVE_HW_BREAKPOINT */
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
	int rc;

	if (data <= 4)
		rc = del_instruction_bp(child, (int)data);
	else
		rc = del_dac(child, (int)data - 4);

	if (!rc) {
		if (!DBCR_ACTIVE_EVENTS(child->thread.debug.dbcr0,
					child->thread.debug.dbcr1)) {
			child->thread.debug.dbcr0 &= ~DBCR0_IDM;
			child->thread.regs->msr &= ~MSR_DE;
		}
	}
	return rc;
#else
	if (data != 1)
		return -EINVAL;

#ifdef CONFIG_HAVE_HW_BREAKPOINT
	bp = thread->ptrace_bps[0];
	if (bp) {
		unregister_hw_breakpoint(bp);
		thread->ptrace_bps[0] = NULL;
	} else
		ret = -ENOENT;
	return ret;
#else /* CONFIG_HAVE_HW_BREAKPOINT */
	if (child->thread.hw_brk.address == 0)
		return -ENOENT;

	child->thread.hw_brk.address = 0;
	child->thread.hw_brk.type = 0;
#endif /* CONFIG_HAVE_HW_BREAKPOINT */

	return 0;
#endif
}
