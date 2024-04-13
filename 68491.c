static int tm_cgpr32_get(struct task_struct *target,
		     const struct user_regset *regset,
		     unsigned int pos, unsigned int count,
		     void *kbuf, void __user *ubuf)
{
	return gpr32_get_common(target, regset, pos, count, kbuf, ubuf,
			&target->thread.ckpt_regs.gpr[0]);
}
