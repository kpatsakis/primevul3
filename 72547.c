static int select_idle_sibling(struct task_struct *p, int prev, int target)
{
	struct sched_domain *sd;
	int i, recent_used_cpu;

	if (available_idle_cpu(target))
		return target;

	/*
	 * If the previous CPU is cache affine and idle, don't be stupid:
	 */
	if (prev != target && cpus_share_cache(prev, target) && available_idle_cpu(prev))
		return prev;

	/* Check a recently used CPU as a potential idle candidate: */
	recent_used_cpu = p->recent_used_cpu;
	if (recent_used_cpu != prev &&
	    recent_used_cpu != target &&
	    cpus_share_cache(recent_used_cpu, target) &&
	    available_idle_cpu(recent_used_cpu) &&
	    cpumask_test_cpu(p->recent_used_cpu, &p->cpus_allowed)) {
		/*
		 * Replace recent_used_cpu with prev as it is a potential
		 * candidate for the next wake:
		 */
		p->recent_used_cpu = prev;
		return recent_used_cpu;
	}

	sd = rcu_dereference(per_cpu(sd_llc, target));
	if (!sd)
		return target;

	i = select_idle_core(p, sd, target);
	if ((unsigned)i < nr_cpumask_bits)
		return i;

	i = select_idle_cpu(p, sd, target);
	if ((unsigned)i < nr_cpumask_bits)
		return i;

	i = select_idle_smt(p, sd, target);
	if ((unsigned)i < nr_cpumask_bits)
		return i;

	return target;
}
