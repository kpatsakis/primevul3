static int select_idle_smt(struct task_struct *p, struct sched_domain *sd, int target)
{
	int cpu;

	if (!static_branch_likely(&sched_smt_present))
		return -1;

	for_each_cpu(cpu, cpu_smt_mask(target)) {
		if (!cpumask_test_cpu(cpu, &p->cpus_allowed))
			continue;
		if (available_idle_cpu(cpu))
			return cpu;
	}

	return -1;
}
