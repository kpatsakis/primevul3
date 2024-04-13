do_device_not_available(struct pt_regs *regs, long error_code)
{
	unsigned long cr0;

	RCU_LOCKDEP_WARN(!rcu_is_watching(), "entry code didn't wake RCU");

#ifdef CONFIG_MATH_EMULATION
	if (!boot_cpu_has(X86_FEATURE_FPU) && (read_cr0() & X86_CR0_EM)) {
		struct math_emu_info info = { };

		cond_local_irq_enable(regs);

		info.regs = regs;
		math_emulate(&info);
		return;
	}
#endif

	/* This should not happen. */
	cr0 = read_cr0();
	if (WARN(cr0 & X86_CR0_TS, "CR0.TS was set")) {
		/* Try to fix it up and carry on. */
		write_cr0(cr0 & ~X86_CR0_TS);
	} else {
		/*
		 * Something terrible happened, and we're better off trying
		 * to kill the task than getting stuck in a never-ending
		 * loop of #NM faults.
		 */
		die("unexpected #NM exception", regs, error_code);
	}
}
