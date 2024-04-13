static int set_user_dscr(struct task_struct *task, unsigned long dscr)
{
	task->thread.dscr = dscr;
	task->thread.dscr_inherit = 1;
	return 0;
}
