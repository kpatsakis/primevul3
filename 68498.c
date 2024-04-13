static int tm_cvsx_active(struct task_struct *target,
				const struct user_regset *regset)
{
	if (!cpu_has_feature(CPU_FTR_TM))
		return -ENODEV;

	if (!MSR_TM_ACTIVE(target->thread.regs->msr))
		return 0;

	flush_vsx_to_thread(target);
	return target->thread.used_vsr ? regset->n : 0;
}
