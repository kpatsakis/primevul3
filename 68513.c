void user_enable_single_step(struct task_struct *task)
{
	struct pt_regs *regs = task->thread.regs;

	if (regs != NULL) {
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
		task->thread.debug.dbcr0 &= ~DBCR0_BT;
		task->thread.debug.dbcr0 |= DBCR0_IDM | DBCR0_IC;
		regs->msr |= MSR_DE;
#else
		regs->msr &= ~MSR_BE;
		regs->msr |= MSR_SE;
#endif
	}
	set_tsk_thread_flag(task, TIF_SINGLESTEP);
}
