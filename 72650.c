static int wake_affine(struct sched_domain *sd, struct task_struct *p,
		       int this_cpu, int prev_cpu, int sync)
{
	int target = nr_cpumask_bits;

	if (sched_feat(WA_IDLE))
		target = wake_affine_idle(this_cpu, prev_cpu, sync);

	if (sched_feat(WA_WEIGHT) && target == nr_cpumask_bits)
		target = wake_affine_weight(sd, p, this_cpu, prev_cpu, sync);

	schedstat_inc(p->se.statistics.nr_wakeups_affine_attempts);
	if (target == nr_cpumask_bits)
		return prev_cpu;

	schedstat_inc(sd->ttwu_move_affine);
	schedstat_inc(p->se.statistics.nr_wakeups_affine);
	return target;
}
