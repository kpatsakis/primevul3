static int pmu_get(struct task_struct *target,
		      const struct user_regset *regset,
		      unsigned int pos, unsigned int count,
		      void *kbuf, void __user *ubuf)
{
	/* Build tests */
	BUILD_BUG_ON(TSO(siar) + sizeof(unsigned long) != TSO(sdar));
	BUILD_BUG_ON(TSO(sdar) + sizeof(unsigned long) != TSO(sier));
	BUILD_BUG_ON(TSO(sier) + sizeof(unsigned long) != TSO(mmcr2));
	BUILD_BUG_ON(TSO(mmcr2) + sizeof(unsigned long) != TSO(mmcr0));

	if (!cpu_has_feature(CPU_FTR_ARCH_207S))
		return -ENODEV;

	return user_regset_copyout(&pos, &count, &kbuf, &ubuf,
			&target->thread.siar, 0,
			5 * sizeof(unsigned long));
}
