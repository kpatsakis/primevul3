void ist_exit(struct pt_regs *regs)
{
	preempt_enable_no_resched();

	if (!user_mode(regs))
		rcu_nmi_exit();
}
