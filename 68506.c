static int tm_spr_get(struct task_struct *target,
		      const struct user_regset *regset,
		      unsigned int pos, unsigned int count,
		      void *kbuf, void __user *ubuf)
{
	int ret;

	/* Build tests */
	BUILD_BUG_ON(TSO(tm_tfhar) + sizeof(u64) != TSO(tm_texasr));
	BUILD_BUG_ON(TSO(tm_texasr) + sizeof(u64) != TSO(tm_tfiar));
	BUILD_BUG_ON(TSO(tm_tfiar) + sizeof(u64) != TSO(ckpt_regs));

	if (!cpu_has_feature(CPU_FTR_TM))
		return -ENODEV;

	/* Flush the states */
	flush_tmregs_to_thread(target);
	flush_fp_to_thread(target);
	flush_altivec_to_thread(target);

	/* TFHAR register */
	ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				&target->thread.tm_tfhar, 0, sizeof(u64));

	/* TEXASR register */
	if (!ret)
		ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				&target->thread.tm_texasr, sizeof(u64),
				2 * sizeof(u64));

	/* TFIAR register */
	if (!ret)
		ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				&target->thread.tm_tfiar,
				2 * sizeof(u64), 3 * sizeof(u64));
	return ret;
}
