static int tm_cvsx_set(struct task_struct *target,
			const struct user_regset *regset,
			unsigned int pos, unsigned int count,
			const void *kbuf, const void __user *ubuf)
{
	u64 buf[32];
	int ret, i;

	if (!cpu_has_feature(CPU_FTR_TM))
		return -ENODEV;

	if (!MSR_TM_ACTIVE(target->thread.regs->msr))
		return -ENODATA;

	/* Flush the state */
	flush_tmregs_to_thread(target);
	flush_fp_to_thread(target);
	flush_altivec_to_thread(target);
	flush_vsx_to_thread(target);

	for (i = 0; i < 32 ; i++)
		buf[i] = target->thread.ckfp_state.fpr[i][TS_VSRLOWOFFSET];

	ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				 buf, 0, 32 * sizeof(double));
	if (!ret)
		for (i = 0; i < 32 ; i++)
			target->thread.ckfp_state.fpr[i][TS_VSRLOWOFFSET] = buf[i];

	return ret;
}
