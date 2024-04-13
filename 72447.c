find_idlest_group(struct sched_domain *sd, struct task_struct *p,
		  int this_cpu, int sd_flag)
{
	struct sched_group *idlest = NULL, *group = sd->groups;
	struct sched_group *most_spare_sg = NULL;
	unsigned long min_runnable_load = ULONG_MAX;
	unsigned long this_runnable_load = ULONG_MAX;
	unsigned long min_avg_load = ULONG_MAX, this_avg_load = ULONG_MAX;
	unsigned long most_spare = 0, this_spare = 0;
	int load_idx = sd->forkexec_idx;
	int imbalance_scale = 100 + (sd->imbalance_pct-100)/2;
	unsigned long imbalance = scale_load_down(NICE_0_LOAD) *
				(sd->imbalance_pct-100) / 100;

	if (sd_flag & SD_BALANCE_WAKE)
		load_idx = sd->wake_idx;

	do {
		unsigned long load, avg_load, runnable_load;
		unsigned long spare_cap, max_spare_cap;
		int local_group;
		int i;

		/* Skip over this group if it has no CPUs allowed */
		if (!cpumask_intersects(sched_group_span(group),
					&p->cpus_allowed))
			continue;

		local_group = cpumask_test_cpu(this_cpu,
					       sched_group_span(group));

		/*
		 * Tally up the load of all CPUs in the group and find
		 * the group containing the CPU with most spare capacity.
		 */
		avg_load = 0;
		runnable_load = 0;
		max_spare_cap = 0;

		for_each_cpu(i, sched_group_span(group)) {
			/* Bias balancing toward CPUs of our domain */
			if (local_group)
				load = source_load(i, load_idx);
			else
				load = target_load(i, load_idx);

			runnable_load += load;

			avg_load += cfs_rq_load_avg(&cpu_rq(i)->cfs);

			spare_cap = capacity_spare_without(i, p);

			if (spare_cap > max_spare_cap)
				max_spare_cap = spare_cap;
		}

		/* Adjust by relative CPU capacity of the group */
		avg_load = (avg_load * SCHED_CAPACITY_SCALE) /
					group->sgc->capacity;
		runnable_load = (runnable_load * SCHED_CAPACITY_SCALE) /
					group->sgc->capacity;

		if (local_group) {
			this_runnable_load = runnable_load;
			this_avg_load = avg_load;
			this_spare = max_spare_cap;
		} else {
			if (min_runnable_load > (runnable_load + imbalance)) {
				/*
				 * The runnable load is significantly smaller
				 * so we can pick this new CPU:
				 */
				min_runnable_load = runnable_load;
				min_avg_load = avg_load;
				idlest = group;
			} else if ((runnable_load < (min_runnable_load + imbalance)) &&
				   (100*min_avg_load > imbalance_scale*avg_load)) {
				/*
				 * The runnable loads are close so take the
				 * blocked load into account through avg_load:
				 */
				min_avg_load = avg_load;
				idlest = group;
			}

			if (most_spare < max_spare_cap) {
				most_spare = max_spare_cap;
				most_spare_sg = group;
			}
		}
	} while (group = group->next, group != sd->groups);

	/*
	 * The cross-over point between using spare capacity or least load
	 * is too conservative for high utilization tasks on partially
	 * utilized systems if we require spare_capacity > task_util(p),
	 * so we allow for some task stuffing by using
	 * spare_capacity > task_util(p)/2.
	 *
	 * Spare capacity can't be used for fork because the utilization has
	 * not been set yet, we must first select a rq to compute the initial
	 * utilization.
	 */
	if (sd_flag & SD_BALANCE_FORK)
		goto skip_spare;

	if (this_spare > task_util(p) / 2 &&
	    imbalance_scale*this_spare > 100*most_spare)
		return NULL;

	if (most_spare > task_util(p) / 2)
		return most_spare_sg;

skip_spare:
	if (!idlest)
		return NULL;

	/*
	 * When comparing groups across NUMA domains, it's possible for the
	 * local domain to be very lightly loaded relative to the remote
	 * domains but "imbalance" skews the comparison making remote CPUs
	 * look much more favourable. When considering cross-domain, add
	 * imbalance to the runnable load on the remote node and consider
	 * staying local.
	 */
	if ((sd->flags & SD_NUMA) &&
	    min_runnable_load + imbalance >= this_runnable_load)
		return NULL;

	if (min_runnable_load > (this_runnable_load + imbalance))
		return NULL;

	if ((this_runnable_load < (min_runnable_load + imbalance)) &&
	     (100*this_avg_load < imbalance_scale*min_avg_load))
		return NULL;

	return idlest;
}
