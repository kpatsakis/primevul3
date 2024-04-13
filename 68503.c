static int tm_ppr_active(struct task_struct *target,
			 const struct user_regset *regset)
{
	if (!cpu_has_feature(CPU_FTR_TM))
		return -ENODEV;

	if (MSR_TM_ACTIVE(target->thread.regs->msr))
		return regset->n;

	return 0;
}
