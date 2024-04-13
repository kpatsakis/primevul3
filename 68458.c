static int dscr_get(struct task_struct *target,
		      const struct user_regset *regset,
		      unsigned int pos, unsigned int count,
		      void *kbuf, void __user *ubuf)
{
	return user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				   &target->thread.dscr, 0, sizeof(u64));
}
