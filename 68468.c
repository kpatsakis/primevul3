static int gpr32_get_common(struct task_struct *target,
		     const struct user_regset *regset,
		     unsigned int pos, unsigned int count,
			    void *kbuf, void __user *ubuf,
			    unsigned long *regs)
{
	compat_ulong_t *k = kbuf;
	compat_ulong_t __user *u = ubuf;
	compat_ulong_t reg;

	pos /= sizeof(reg);
	count /= sizeof(reg);

	if (kbuf)
		for (; count > 0 && pos < PT_MSR; --count)
			*k++ = regs[pos++];
	else
		for (; count > 0 && pos < PT_MSR; --count)
			if (__put_user((compat_ulong_t) regs[pos++], u++))
				return -EFAULT;

	if (count > 0 && pos == PT_MSR) {
		reg = get_user_msr(target);
		if (kbuf)
			*k++ = reg;
		else if (__put_user(reg, u++))
			return -EFAULT;
		++pos;
		--count;
	}

	if (kbuf)
		for (; count > 0 && pos < PT_REGS_COUNT; --count)
			*k++ = regs[pos++];
	else
		for (; count > 0 && pos < PT_REGS_COUNT; --count)
			if (__put_user((compat_ulong_t) regs[pos++], u++))
				return -EFAULT;

	kbuf = k;
	ubuf = u;
	pos *= sizeof(reg);
	count *= sizeof(reg);
	return user_regset_copyout_zero(&pos, &count, &kbuf, &ubuf,
					PT_REGS_COUNT * sizeof(reg), -1);
}
