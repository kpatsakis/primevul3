static int tm_cgpr32_set(struct task_struct *target,
		     const struct user_regset *regset,
		     unsigned int pos, unsigned int count,
		     const void *kbuf, const void __user *ubuf)
{
	return gpr32_set_common(target, regset, pos, count, kbuf, ubuf,
			&target->thread.ckpt_regs.gpr[0]);
}
