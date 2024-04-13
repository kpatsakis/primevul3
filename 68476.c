int ptrace_get_reg(struct task_struct *task, int regno, unsigned long *data)
{
	if ((task->thread.regs == NULL) || !data)
		return -EIO;

	if (regno == PT_MSR) {
		*data = get_user_msr(task);
		return 0;
	}

	if (regno == PT_DSCR)
		return get_user_dscr(task, data);

	if (regno < (sizeof(struct pt_regs) / sizeof(unsigned long))) {
		*data = ((unsigned long *)task->thread.regs)[regno];
		return 0;
	}

	return -EIO;
}
