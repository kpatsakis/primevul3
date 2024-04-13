static int fpr_set(struct task_struct *target, const struct user_regset *regset,
		   unsigned int pos, unsigned int count,
		   const void *kbuf, const void __user *ubuf)
{
#ifdef CONFIG_VSX
	u64 buf[33];
	int i;

	flush_fp_to_thread(target);

	for (i = 0; i < 32 ; i++)
		buf[i] = target->thread.TS_FPR(i);
	buf[32] = target->thread.fp_state.fpscr;

	/* copy to local buffer then write that out */
	i = user_regset_copyin(&pos, &count, &kbuf, &ubuf, buf, 0, -1);
	if (i)
		return i;

	for (i = 0; i < 32 ; i++)
		target->thread.TS_FPR(i) = buf[i];
	target->thread.fp_state.fpscr = buf[32];
	return 0;
#else
	BUILD_BUG_ON(offsetof(struct thread_fp_state, fpscr) !=
		     offsetof(struct thread_fp_state, fpr[32]));

	flush_fp_to_thread(target);

	return user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				  &target->thread.fp_state, 0, -1);
#endif
}
