static int find_energy_efficient_cpu(struct task_struct *p, int prev_cpu)
{
	unsigned long prev_energy = ULONG_MAX, best_energy = ULONG_MAX;
	struct root_domain *rd = cpu_rq(smp_processor_id())->rd;
	int cpu, best_energy_cpu = prev_cpu;
	struct perf_domain *head, *pd;
	unsigned long cpu_cap, util;
	struct sched_domain *sd;

	rcu_read_lock();
	pd = rcu_dereference(rd->pd);
	if (!pd || READ_ONCE(rd->overutilized))
		goto fail;
	head = pd;

	/*
	 * Energy-aware wake-up happens on the lowest sched_domain starting
	 * from sd_asym_cpucapacity spanning over this_cpu and prev_cpu.
	 */
	sd = rcu_dereference(*this_cpu_ptr(&sd_asym_cpucapacity));
	while (sd && !cpumask_test_cpu(prev_cpu, sched_domain_span(sd)))
		sd = sd->parent;
	if (!sd)
		goto fail;

	sync_entity_load_avg(&p->se);
	if (!task_util_est(p))
		goto unlock;

	for (; pd; pd = pd->next) {
		unsigned long cur_energy, spare_cap, max_spare_cap = 0;
		int max_spare_cap_cpu = -1;

		for_each_cpu_and(cpu, perf_domain_span(pd), sched_domain_span(sd)) {
			if (!cpumask_test_cpu(cpu, &p->cpus_allowed))
				continue;

			/* Skip CPUs that will be overutilized. */
			util = cpu_util_next(cpu, p, cpu);
			cpu_cap = capacity_of(cpu);
			if (cpu_cap * 1024 < util * capacity_margin)
				continue;

			/* Always use prev_cpu as a candidate. */
			if (cpu == prev_cpu) {
				prev_energy = compute_energy(p, prev_cpu, head);
				best_energy = min(best_energy, prev_energy);
				continue;
			}

			/*
			 * Find the CPU with the maximum spare capacity in
			 * the performance domain
			 */
			spare_cap = cpu_cap - util;
			if (spare_cap > max_spare_cap) {
				max_spare_cap = spare_cap;
				max_spare_cap_cpu = cpu;
			}
		}

		/* Evaluate the energy impact of using this CPU. */
		if (max_spare_cap_cpu >= 0) {
			cur_energy = compute_energy(p, max_spare_cap_cpu, head);
			if (cur_energy < best_energy) {
				best_energy = cur_energy;
				best_energy_cpu = max_spare_cap_cpu;
			}
		}
	}
unlock:
	rcu_read_unlock();

	/*
	 * Pick the best CPU if prev_cpu cannot be used, or if it saves at
	 * least 6% of the energy used by prev_cpu.
	 */
	if (prev_energy == ULONG_MAX)
		return best_energy_cpu;

	if ((prev_energy - best_energy) > (prev_energy >> 4))
		return best_energy_cpu;

	return prev_cpu;

fail:
	rcu_read_unlock();

	return -1;
}
