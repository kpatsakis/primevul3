static int ptrace_detach(struct task_struct *child, unsigned int data)
{
	if (!valid_signal(data))
		return -EIO;

	/* Architecture-specific hardware disable .. */
	ptrace_disable(child);

	write_lock_irq(&tasklist_lock);
	/*
	 * We rely on ptrace_freeze_traced(). It can't be killed and
	 * untraced by another thread, it can't be a zombie.
	 */
	WARN_ON(!child->ptrace || child->exit_state);
	/*
	 * tasklist_lock avoids the race with wait_task_stopped(), see
	 * the comment in ptrace_resume().
	 */
	child->exit_code = data;
	__ptrace_detach(current, child);
	write_unlock_irq(&tasklist_lock);

	proc_ptrace_connector(child, PTRACE_DETACH);

	return 0;
}
