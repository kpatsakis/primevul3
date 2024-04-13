static int tm_cvmx_set(struct task_struct *target,
			const struct user_regset *regset,
			unsigned int pos, unsigned int count,
			const void *kbuf, const void __user *ubuf)
{
	int ret;

	BUILD_BUG_ON(TVSO(vscr) != TVSO(vr[32]));

	if (!cpu_has_feature(CPU_FTR_TM))
		return -ENODEV;

	if (!MSR_TM_ACTIVE(target->thread.regs->msr))
		return -ENODATA;

	flush_tmregs_to_thread(target);
	flush_fp_to_thread(target);
	flush_altivec_to_thread(target);

	ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
					&target->thread.ckvr_state, 0,
					33 * sizeof(vector128));
	if (!ret && count > 0) {
		/*
		 * We use only the low-order word of vrsave.
		 */
		union {
			elf_vrreg_t reg;
			u32 word;
		} vrsave;
		memset(&vrsave, 0, sizeof(vrsave));
		vrsave.word = target->thread.ckvrsave;
		ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf, &vrsave,
						33 * sizeof(vector128), -1);
		if (!ret)
			target->thread.ckvrsave = vrsave.word;
	}

	return ret;
}
