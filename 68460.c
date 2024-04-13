static int ebb_active(struct task_struct *target,
			 const struct user_regset *regset)
{
	if (!cpu_has_feature(CPU_FTR_ARCH_207S))
		return -ENODEV;

	if (target->thread.used_ebb)
		return regset->n;

	return 0;
}
