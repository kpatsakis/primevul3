static int ppr_set(struct task_struct *target,
		      const struct user_regset *regset,
		      unsigned int pos, unsigned int count,
		      const void *kbuf, const void __user *ubuf)
{
	return user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				  &target->thread.ppr, 0, sizeof(u64));
}
