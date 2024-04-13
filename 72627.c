static bool update_nohz_stats(struct rq *rq, bool force)
{
#ifdef CONFIG_NO_HZ_COMMON
	unsigned int cpu = rq->cpu;

	if (!rq->has_blocked_load)
		return false;

	if (!cpumask_test_cpu(cpu, nohz.idle_cpus_mask))
		return false;

	if (!force && !time_after(jiffies, rq->last_blocked_load_update_tick))
		return true;

	update_blocked_averages(cpu);

	return rq->has_blocked_load;
#else
	return false;
#endif
}
