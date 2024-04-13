static void nohz_newidle_balance(struct rq *this_rq)
{
	int this_cpu = this_rq->cpu;

	/*
	 * This CPU doesn't want to be disturbed by scheduler
	 * housekeeping
	 */
	if (!housekeeping_cpu(this_cpu, HK_FLAG_SCHED))
		return;

	/* Will wake up very soon. No time for doing anything else*/
	if (this_rq->avg_idle < sysctl_sched_migration_cost)
		return;

	/* Don't need to update blocked load of idle CPUs*/
	if (!READ_ONCE(nohz.has_blocked) ||
	    time_before(jiffies, READ_ONCE(nohz.next_blocked)))
		return;

	raw_spin_unlock(&this_rq->lock);
	/*
	 * This CPU is going to be idle and blocked load of idle CPUs
	 * need to be updated. Run the ilb locally as it is a good
	 * candidate for ilb instead of waking up another idle CPU.
	 * Kick an normal ilb if we failed to do the update.
	 */
	if (!_nohz_idle_balance(this_rq, NOHZ_STATS_KICK, CPU_NEWLY_IDLE))
		kick_ilb(NOHZ_STATS_KICK);
	raw_spin_lock(&this_rq->lock);
}
