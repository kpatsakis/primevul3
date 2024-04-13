static void kick_ilb(unsigned int flags)
{
	int ilb_cpu;

	nohz.next_balance++;

	ilb_cpu = find_new_ilb();

	if (ilb_cpu >= nr_cpu_ids)
		return;

	flags = atomic_fetch_or(flags, nohz_flags(ilb_cpu));
	if (flags & NOHZ_KICK_MASK)
		return;

	/*
	 * Use smp_send_reschedule() instead of resched_cpu().
	 * This way we generate a sched IPI on the target CPU which
	 * is idle. And the softirq performing nohz idle load balance
	 * will be run before returning from the IPI.
	 */
	smp_send_reschedule(ilb_cpu);
}
