static int ebb_get(struct task_struct *target,
		      const struct user_regset *regset,
		      unsigned int pos, unsigned int count,
		      void *kbuf, void __user *ubuf)
{
	/* Build tests */
	BUILD_BUG_ON(TSO(ebbrr) + sizeof(unsigned long) != TSO(ebbhr));
	BUILD_BUG_ON(TSO(ebbhr) + sizeof(unsigned long) != TSO(bescr));

	if (!cpu_has_feature(CPU_FTR_ARCH_207S))
		return -ENODEV;

	if (!target->thread.used_ebb)
		return -ENODATA;

	return user_regset_copyout(&pos, &count, &kbuf, &ubuf,
			&target->thread.ebbrr, 0, 3 * sizeof(unsigned long));
}
