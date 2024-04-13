static int ptrace_resume(struct task_struct *child, long request,
			 unsigned long data)
{
	bool need_siglock;

	if (!valid_signal(data))
		return -EIO;

	if (request == PTRACE_SYSCALL)
		set_tsk_thread_flag(child, TIF_SYSCALL_TRACE);
	else
		clear_tsk_thread_flag(child, TIF_SYSCALL_TRACE);

#ifdef TIF_SYSCALL_EMU
	if (request == PTRACE_SYSEMU || request == PTRACE_SYSEMU_SINGLESTEP)
		set_tsk_thread_flag(child, TIF_SYSCALL_EMU);
	else
		clear_tsk_thread_flag(child, TIF_SYSCALL_EMU);
#endif

	if (is_singleblock(request)) {
		if (unlikely(!arch_has_block_step()))
			return -EIO;
		user_enable_block_step(child);
	} else if (is_singlestep(request) || is_sysemu_singlestep(request)) {
		if (unlikely(!arch_has_single_step()))
			return -EIO;
		user_enable_single_step(child);
	} else {
		user_disable_single_step(child);
	}

	/*
	 * Change ->exit_code and ->state under siglock to avoid the race
	 * with wait_task_stopped() in between; a non-zero ->exit_code will
	 * wrongly look like another report from tracee.
	 *
	 * Note that we need siglock even if ->exit_code == data and/or this
	 * status was not reported yet, the new status must not be cleared by
	 * wait_task_stopped() after resume.
	 *
	 * If data == 0 we do not care if wait_task_stopped() reports the old
	 * status and clears the code too; this can't race with the tracee, it
	 * takes siglock after resume.
	 */
	need_siglock = data && !thread_group_empty(current);
	if (need_siglock)
		spin_lock_irq(&child->sighand->siglock);
	child->exit_code = data;
	wake_up_state(child, __TASK_TRACED);
	if (need_siglock)
		spin_unlock_irq(&child->sighand->siglock);

	return 0;
}
