static int pmu_set(struct task_struct *target,
		      const struct user_regset *regset,
		      unsigned int pos, unsigned int count,
		      const void *kbuf, const void __user *ubuf)
{
	int ret = 0;

	/* Build tests */
	BUILD_BUG_ON(TSO(siar) + sizeof(unsigned long) != TSO(sdar));
	BUILD_BUG_ON(TSO(sdar) + sizeof(unsigned long) != TSO(sier));
	BUILD_BUG_ON(TSO(sier) + sizeof(unsigned long) != TSO(mmcr2));
	BUILD_BUG_ON(TSO(mmcr2) + sizeof(unsigned long) != TSO(mmcr0));

	if (!cpu_has_feature(CPU_FTR_ARCH_207S))
		return -ENODEV;

	ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
			&target->thread.siar, 0,
			sizeof(unsigned long));

	if (!ret)
		ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
			&target->thread.sdar, sizeof(unsigned long),
			2 * sizeof(unsigned long));

	if (!ret)
		ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
			&target->thread.sier, 2 * sizeof(unsigned long),
			3 * sizeof(unsigned long));

	if (!ret)
		ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
			&target->thread.mmcr2, 3 * sizeof(unsigned long),
			4 * sizeof(unsigned long));

	if (!ret)
		ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
			&target->thread.mmcr0, 4 * sizeof(unsigned long),
			5 * sizeof(unsigned long));
	return ret;
}
