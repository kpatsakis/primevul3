static int gpr32_get(struct task_struct *target,
		     const struct user_regset *regset,
		     unsigned int pos, unsigned int count,
		     void *kbuf, void __user *ubuf)
{
	int i;

	if (target->thread.regs == NULL)
		return -EIO;

	if (!FULL_REGS(target->thread.regs)) {
		/*
		 * We have a partial register set.
		 * Fill 14-31 with bogus values.
		 */
		for (i = 14; i < 32; i++)
			target->thread.regs->gpr[i] = NV_REG_POISON;
	}
	return gpr32_get_common(target, regset, pos, count, kbuf, ubuf,
			&target->thread.regs->gpr[0]);
}
