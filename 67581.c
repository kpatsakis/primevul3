void ist_enter(struct pt_regs *regs)
{
	if (user_mode(regs)) {
		RCU_LOCKDEP_WARN(!rcu_is_watching(), "entry code didn't wake RCU");
	} else {
		/*
		 * We might have interrupted pretty much anything.  In
		 * fact, if we're a machine check, we can even interrupt
		 * NMI processing.  We don't want in_nmi() to return true,
		 * but we need to notify RCU.
		 */
		rcu_nmi_enter();
	}

	preempt_disable();

	/* This code is a bit fragile.  Test it. */
	RCU_LOCKDEP_WARN(!rcu_is_watching(), "ist_enter didn't work");
}
