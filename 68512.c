void user_enable_block_step(struct task_struct *task)
{
	struct pt_regs *regs = task->thread.regs;

	if (regs != NULL) {
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
		task->thread.debug.dbcr0 &= ~DBCR0_IC;
		task->thread.debug.dbcr0 = DBCR0_IDM | DBCR0_BT;
		regs->msr |= MSR_DE;
#else
		regs->msr &= ~MSR_SE;
		regs->msr |= MSR_BE;
#endif
	}
	set_tsk_thread_flag(task, TIF_SINGLESTEP);
}
