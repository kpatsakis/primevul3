static int select_idle_cpu(struct task_struct *p, struct sched_domain *sd, int target)
{
	struct sched_domain *this_sd;
	u64 avg_cost, avg_idle;
	u64 time, cost;
	s64 delta;
	int cpu, nr = INT_MAX;

	this_sd = rcu_dereference(*this_cpu_ptr(&sd_llc));
	if (!this_sd)
		return -1;

	/*
	 * Due to large variance we need a large fuzz factor; hackbench in
	 * particularly is sensitive here.
	 */
	avg_idle = this_rq()->avg_idle / 512;
	avg_cost = this_sd->avg_scan_cost + 1;

	if (sched_feat(SIS_AVG_CPU) && avg_idle < avg_cost)
		return -1;

	if (sched_feat(SIS_PROP)) {
		u64 span_avg = sd->span_weight * avg_idle;
		if (span_avg > 4*avg_cost)
			nr = div_u64(span_avg, avg_cost);
		else
			nr = 4;
	}

	time = local_clock();

	for_each_cpu_wrap(cpu, sched_domain_span(sd), target) {
		if (!--nr)
			return -1;
		if (!cpumask_test_cpu(cpu, &p->cpus_allowed))
			continue;
		if (available_idle_cpu(cpu))
			break;
	}

	time = local_clock() - time;
	cost = this_sd->avg_scan_cost;
	delta = (s64)(time - cost) / 8;
	this_sd->avg_scan_cost += delta;

	return cpu;
}
