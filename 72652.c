static int wake_cap(struct task_struct *p, int cpu, int prev_cpu)
{
	long min_cap, max_cap;

	if (!static_branch_unlikely(&sched_asym_cpucapacity))
		return 0;

	min_cap = min(capacity_orig_of(prev_cpu), capacity_orig_of(cpu));
	max_cap = cpu_rq(cpu)->rd->max_cpu_capacity;

	/* Minimum capacity is close to max, no need to abort wake_affine */
	if (max_cap - min_cap < max_cap >> 3)
		return 0;

	/* Bring task utilization in sync with prev_cpu */
	sync_entity_load_avg(&p->se);

	return !task_fits_capacity(p, min_cap);
}
