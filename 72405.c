static unsigned long cpu_avg_load_per_task(int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long nr_running = READ_ONCE(rq->cfs.h_nr_running);
	unsigned long load_avg = weighted_cpuload(rq);

	if (nr_running)
		return load_avg / nr_running;

	return 0;
}
