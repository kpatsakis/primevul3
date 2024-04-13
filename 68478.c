static int ptrace_set_debugreg(struct task_struct *task, unsigned long addr,
			       unsigned long data)
{
#ifdef CONFIG_HAVE_HW_BREAKPOINT
	int ret;
	struct thread_struct *thread = &(task->thread);
	struct perf_event *bp;
	struct perf_event_attr attr;
#endif /* CONFIG_HAVE_HW_BREAKPOINT */
#ifndef CONFIG_PPC_ADV_DEBUG_REGS
	struct arch_hw_breakpoint hw_brk;
#endif

	/* For ppc64 we support one DABR and no IABR's at the moment (ppc64).
	 *  For embedded processors we support one DAC and no IAC's at the
	 *  moment.
	 */
	if (addr > 0)
		return -EINVAL;

	/* The bottom 3 bits in dabr are flags */
	if ((data & ~0x7UL) >= TASK_SIZE)
		return -EIO;

#ifndef CONFIG_PPC_ADV_DEBUG_REGS
	/* For processors using DABR (i.e. 970), the bottom 3 bits are flags.
	 *  It was assumed, on previous implementations, that 3 bits were
	 *  passed together with the data address, fitting the design of the
	 *  DABR register, as follows:
	 *
	 *  bit 0: Read flag
	 *  bit 1: Write flag
	 *  bit 2: Breakpoint translation
	 *
	 *  Thus, we use them here as so.
	 */

	/* Ensure breakpoint translation bit is set */
	if (data && !(data & HW_BRK_TYPE_TRANSLATE))
		return -EIO;
	hw_brk.address = data & (~HW_BRK_TYPE_DABR);
	hw_brk.type = (data & HW_BRK_TYPE_DABR) | HW_BRK_TYPE_PRIV_ALL;
	hw_brk.len = 8;
#ifdef CONFIG_HAVE_HW_BREAKPOINT
	bp = thread->ptrace_bps[0];
	if ((!data) || !(hw_brk.type & HW_BRK_TYPE_RDWR)) {
		if (bp) {
			unregister_hw_breakpoint(bp);
			thread->ptrace_bps[0] = NULL;
		}
		return 0;
	}
	if (bp) {
		attr = bp->attr;
		attr.bp_addr = hw_brk.address;
		arch_bp_generic_fields(hw_brk.type, &attr.bp_type);

		/* Enable breakpoint */
		attr.disabled = false;

		ret =  modify_user_hw_breakpoint(bp, &attr);
		if (ret) {
			return ret;
		}
		thread->ptrace_bps[0] = bp;
		thread->hw_brk = hw_brk;
		return 0;
	}

	/* Create a new breakpoint request if one doesn't exist already */
	hw_breakpoint_init(&attr);
	attr.bp_addr = hw_brk.address;
	arch_bp_generic_fields(hw_brk.type,
			       &attr.bp_type);

	thread->ptrace_bps[0] = bp = register_user_hw_breakpoint(&attr,
					       ptrace_triggered, NULL, task);
	if (IS_ERR(bp)) {
		thread->ptrace_bps[0] = NULL;
		return PTR_ERR(bp);
	}

#endif /* CONFIG_HAVE_HW_BREAKPOINT */
	task->thread.hw_brk = hw_brk;
#else /* CONFIG_PPC_ADV_DEBUG_REGS */
	/* As described above, it was assumed 3 bits were passed with the data
	 *  address, but we will assume only the mode bits will be passed
	 *  as to not cause alignment restrictions for DAC-based processors.
	 */

	/* DAC's hold the whole address without any mode flags */
	task->thread.debug.dac1 = data & ~0x3UL;

	if (task->thread.debug.dac1 == 0) {
		dbcr_dac(task) &= ~(DBCR_DAC1R | DBCR_DAC1W);
		if (!DBCR_ACTIVE_EVENTS(task->thread.debug.dbcr0,
					task->thread.debug.dbcr1)) {
			task->thread.regs->msr &= ~MSR_DE;
			task->thread.debug.dbcr0 &= ~DBCR0_IDM;
		}
		return 0;
	}

	/* Read or Write bits must be set */

	if (!(data & 0x3UL))
		return -EINVAL;

	/* Set the Internal Debugging flag (IDM bit 1) for the DBCR0
	   register */
	task->thread.debug.dbcr0 |= DBCR0_IDM;

	/* Check for write and read flags and set DBCR0
	   accordingly */
	dbcr_dac(task) &= ~(DBCR_DAC1R|DBCR_DAC1W);
	if (data & 0x1UL)
		dbcr_dac(task) |= DBCR_DAC1R;
	if (data & 0x2UL)
		dbcr_dac(task) |= DBCR_DAC1W;
	task->thread.regs->msr |= MSR_DE;
#endif /* CONFIG_PPC_ADV_DEBUG_REGS */
	return 0;
}
