static int vr_set(struct task_struct *target, const struct user_regset *regset,
		  unsigned int pos, unsigned int count,
		  const void *kbuf, const void __user *ubuf)
{
	int ret;

	flush_altivec_to_thread(target);

	BUILD_BUG_ON(offsetof(struct thread_vr_state, vscr) !=
		     offsetof(struct thread_vr_state, vr[32]));

	ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				 &target->thread.vr_state, 0,
				 33 * sizeof(vector128));
	if (!ret && count > 0) {
		/*
		 * We use only the first word of vrsave.
		 */
		union {
			elf_vrreg_t reg;
			u32 word;
		} vrsave;
		memset(&vrsave, 0, sizeof(vrsave));

		vrsave.word = target->thread.vrsave;

		ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf, &vrsave,
					 33 * sizeof(vector128), -1);
		if (!ret)
			target->thread.vrsave = vrsave.word;
	}

	return ret;
}
