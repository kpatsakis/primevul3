static int vr_get(struct task_struct *target, const struct user_regset *regset,
		  unsigned int pos, unsigned int count,
		  void *kbuf, void __user *ubuf)
{
	int ret;

	flush_altivec_to_thread(target);

	BUILD_BUG_ON(offsetof(struct thread_vr_state, vscr) !=
		     offsetof(struct thread_vr_state, vr[32]));

	ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				  &target->thread.vr_state, 0,
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

		vrsave.word = target->thread.vrsave;

		ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf, &vrsave,
					  33 * sizeof(vector128), -1);
	}

	return ret;
}
