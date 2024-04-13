static int select_idle_core(struct task_struct *p, struct sched_domain *sd, int target)
{
	struct cpumask *cpus = this_cpu_cpumask_var_ptr(select_idle_mask);
	int core, cpu;

	if (!static_branch_likely(&sched_smt_present))
		return -1;

	if (!test_idle_cores(target, false))
		return -1;

	cpumask_and(cpus, sched_domain_span(sd), &p->cpus_allowed);

	for_each_cpu_wrap(core, cpus, target) {
		bool idle = true;

		for_each_cpu(cpu, cpu_smt_mask(core)) {
			cpumask_clear_cpu(cpu, cpus);
			if (!available_idle_cpu(cpu))
				idle = false;
		}

		if (idle)
			return core;
	}

	/*
	 * Failed to find an idle core; stop looking for one.
	 */
	set_idle_cores(target, 0);

	return -1;
}
