static int vsr_get(struct task_struct *target, const struct user_regset *regset,
		   unsigned int pos, unsigned int count,
		   void *kbuf, void __user *ubuf)
{
	u64 buf[32];
	int ret, i;

	flush_tmregs_to_thread(target);
	flush_fp_to_thread(target);
	flush_altivec_to_thread(target);
	flush_vsx_to_thread(target);

	for (i = 0; i < 32 ; i++)
		buf[i] = target->thread.fp_state.fpr[i][TS_VSRLOWOFFSET];

	ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				  buf, 0, 32 * sizeof(double));

	return ret;
}
