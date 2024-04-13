static int tm_cvmx_get(struct task_struct *target,
			const struct user_regset *regset,
			unsigned int pos, unsigned int count,
			void *kbuf, void __user *ubuf)
{
	int ret;

	BUILD_BUG_ON(TVSO(vscr) != TVSO(vr[32]));

	if (!cpu_has_feature(CPU_FTR_TM))
		return -ENODEV;

	if (!MSR_TM_ACTIVE(target->thread.regs->msr))
		return -ENODATA;

	/* Flush the state */
	flush_tmregs_to_thread(target);
	flush_fp_to_thread(target);
	flush_altivec_to_thread(target);

	ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
					&target->thread.ckvr_state, 0,
					33 * sizeof(vector128));
	if (!ret) {
		/*
		 * Copy out only the low-order word of vrsave.
		 */
		union {
			elf_vrreg_t reg;
			u32 word;
		} vrsave;
		memset(&vrsave, 0, sizeof(vrsave));
		vrsave.word = target->thread.ckvrsave;
		ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf, &vrsave,
						33 * sizeof(vector128), -1);
	}

	return ret;
}
