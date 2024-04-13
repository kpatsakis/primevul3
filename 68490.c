static int tm_cfpr_set(struct task_struct *target,
			const struct user_regset *regset,
			unsigned int pos, unsigned int count,
			const void *kbuf, const void __user *ubuf)
{
	u64 buf[33];
	int i;

	if (!cpu_has_feature(CPU_FTR_TM))
		return -ENODEV;

	if (!MSR_TM_ACTIVE(target->thread.regs->msr))
		return -ENODATA;

	flush_tmregs_to_thread(target);
	flush_fp_to_thread(target);
	flush_altivec_to_thread(target);

	for (i = 0; i < 32; i++)
		buf[i] = target->thread.TS_CKFPR(i);
	buf[32] = target->thread.ckfp_state.fpscr;

	/* copy to local buffer then write that out */
	i = user_regset_copyin(&pos, &count, &kbuf, &ubuf, buf, 0, -1);
	if (i)
		return i;
	for (i = 0; i < 32 ; i++)
		target->thread.TS_CKFPR(i) = buf[i];
	target->thread.ckfp_state.fpscr = buf[32];
	return 0;
}
