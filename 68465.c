static int get_user_dscr(struct task_struct *task, unsigned long *data)
{
	*data = task->thread.dscr;
	return 0;
}
