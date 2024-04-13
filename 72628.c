static void update_numa_stats(struct numa_stats *ns, int nid)
{
	int cpu;

	memset(ns, 0, sizeof(*ns));
	for_each_cpu(cpu, cpumask_of_node(nid)) {
		struct rq *rq = cpu_rq(cpu);

		ns->load += weighted_cpuload(rq);
		ns->compute_capacity += capacity_of(cpu);
	}

}
