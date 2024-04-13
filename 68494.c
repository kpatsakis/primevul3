static int tm_cgpr_get(struct task_struct *target,
			const struct user_regset *regset,
			unsigned int pos, unsigned int count,
			void *kbuf, void __user *ubuf)
{
	int ret;

	if (!cpu_has_feature(CPU_FTR_TM))
		return -ENODEV;

	if (!MSR_TM_ACTIVE(target->thread.regs->msr))
		return -ENODATA;

	flush_tmregs_to_thread(target);
	flush_fp_to_thread(target);
	flush_altivec_to_thread(target);

	ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				  &target->thread.ckpt_regs,
				  0, offsetof(struct pt_regs, msr));
	if (!ret) {
		unsigned long msr = get_user_ckpt_msr(target);

		ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf, &msr,
					  offsetof(struct pt_regs, msr),
					  offsetof(struct pt_regs, msr) +
					  sizeof(msr));
	}

	BUILD_BUG_ON(offsetof(struct pt_regs, orig_gpr3) !=
		     offsetof(struct pt_regs, msr) + sizeof(long));

	if (!ret)
		ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
					  &target->thread.ckpt_regs.orig_gpr3,
					  offsetof(struct pt_regs, orig_gpr3),
					  sizeof(struct pt_regs));
	if (!ret)
		ret = user_regset_copyout_zero(&pos, &count, &kbuf, &ubuf,
					       sizeof(struct pt_regs), -1);

	return ret;
}
