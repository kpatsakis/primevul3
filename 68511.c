void user_disable_single_step(struct task_struct *task)
{
	struct pt_regs *regs = task->thread.regs;

	if (regs != NULL) {
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
		/*
		 * The logic to disable single stepping should be as
		 * simple as turning off the Instruction Complete flag.
		 * And, after doing so, if all debug flags are off, turn
		 * off DBCR0(IDM) and MSR(DE) .... Torez
		 */
		task->thread.debug.dbcr0 &= ~(DBCR0_IC|DBCR0_BT);
		/*
		 * Test to see if any of the DBCR_ACTIVE_EVENTS bits are set.
		 */
		if (!DBCR_ACTIVE_EVENTS(task->thread.debug.dbcr0,
					task->thread.debug.dbcr1)) {
			/*
			 * All debug events were off.....
			 */
			task->thread.debug.dbcr0 &= ~DBCR0_IDM;
			regs->msr &= ~MSR_DE;
		}
#else
		regs->msr &= ~(MSR_SE | MSR_BE);
#endif
	}
	clear_tsk_thread_flag(task, TIF_SINGLESTEP);
}
