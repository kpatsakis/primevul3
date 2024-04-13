static void cpu_load_update_idle(struct rq *this_rq)
{
	/*
	 * bail if there's load or we're actually up-to-date.
	 */
	if (weighted_cpuload(this_rq))
		return;

	cpu_load_update_nohz(this_rq, READ_ONCE(jiffies), 0);
}
