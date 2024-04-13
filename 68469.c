static int gpr32_set(struct task_struct *target,
		     const struct user_regset *regset,
		     unsigned int pos, unsigned int count,
		     const void *kbuf, const void __user *ubuf)
{
	if (target->thread.regs == NULL)
		return -EIO;

	CHECK_FULL_REGS(target->thread.regs);
	return gpr32_set_common(target, regset, pos, count, kbuf, ubuf,
			&target->thread.regs->gpr[0]);
}
