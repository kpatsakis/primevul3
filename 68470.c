static int gpr32_set_common(struct task_struct *target,
		     const struct user_regset *regset,
		     unsigned int pos, unsigned int count,
		     const void *kbuf, const void __user *ubuf,
		     unsigned long *regs)
{
	const compat_ulong_t *k = kbuf;
	const compat_ulong_t __user *u = ubuf;
	compat_ulong_t reg;

	pos /= sizeof(reg);
	count /= sizeof(reg);

	if (kbuf)
		for (; count > 0 && pos < PT_MSR; --count)
			regs[pos++] = *k++;
	else
		for (; count > 0 && pos < PT_MSR; --count) {
			if (__get_user(reg, u++))
				return -EFAULT;
			regs[pos++] = reg;
		}


	if (count > 0 && pos == PT_MSR) {
		if (kbuf)
			reg = *k++;
		else if (__get_user(reg, u++))
			return -EFAULT;
		set_user_msr(target, reg);
		++pos;
		--count;
	}

	if (kbuf) {
		for (; count > 0 && pos <= PT_MAX_PUT_REG; --count)
			regs[pos++] = *k++;
		for (; count > 0 && pos < PT_TRAP; --count, ++pos)
			++k;
	} else {
		for (; count > 0 && pos <= PT_MAX_PUT_REG; --count) {
			if (__get_user(reg, u++))
				return -EFAULT;
			regs[pos++] = reg;
		}
		for (; count > 0 && pos < PT_TRAP; --count, ++pos)
			if (__get_user(reg, u++))
				return -EFAULT;
	}

	if (count > 0 && pos == PT_TRAP) {
		if (kbuf)
			reg = *k++;
		else if (__get_user(reg, u++))
			return -EFAULT;
		set_user_trap(target, reg);
		++pos;
		--count;
	}

	kbuf = k;
	ubuf = u;
	pos *= sizeof(reg);
	count *= sizeof(reg);
	return user_regset_copyin_ignore(&pos, &count, &kbuf, &ubuf,
					 (PT_TRAP + 1) * sizeof(reg), -1);
}
