static unsigned long source_load(int cpu, int type)
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long total = weighted_cpuload(rq);

	if (type == 0 || !sched_feat(LB_BIAS))
		return total;

	return min(rq->cpu_load[type-1], total);
}
