static int tm_tar_set(struct task_struct *target,
		      const struct user_regset *regset,
		      unsigned int pos, unsigned int count,
		      const void *kbuf, const void __user *ubuf)
{
	int ret;

	if (!cpu_has_feature(CPU_FTR_TM))
		return -ENODEV;

	if (!MSR_TM_ACTIVE(target->thread.regs->msr))
		return -ENODATA;

	ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				&target->thread.tm_tar, 0, sizeof(u64));
	return ret;
}
