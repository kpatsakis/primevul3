static int set_user_ckpt_msr(struct task_struct *task, unsigned long msr)
{
	task->thread.ckpt_regs.msr &= ~MSR_DEBUGCHANGE;
	task->thread.ckpt_regs.msr |= msr & MSR_DEBUGCHANGE;
	return 0;
}
