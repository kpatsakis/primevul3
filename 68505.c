static int tm_spr_active(struct task_struct *target,
			 const struct user_regset *regset)
{
	if (!cpu_has_feature(CPU_FTR_TM))
		return -ENODEV;

	return regset->n;
}
