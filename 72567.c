static unsigned long target_load(int cpu, int type)
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long total = weighted_cpuload(rq);

	if (type == 0 || !sched_feat(LB_BIAS))
		return total;

	return max(rq->cpu_load[type-1], total);
}
