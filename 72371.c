static inline void calculate_imbalance(struct lb_env *env, struct sd_lb_stats *sds)
{
	unsigned long max_pull, load_above_capacity = ~0UL;
	struct sg_lb_stats *local, *busiest;

	local = &sds->local_stat;
	busiest = &sds->busiest_stat;

	if (busiest->group_type == group_imbalanced) {
		/*
		 * In the group_imb case we cannot rely on group-wide averages
		 * to ensure CPU-load equilibrium, look at wider averages. XXX
		 */
		busiest->load_per_task =
			min(busiest->load_per_task, sds->avg_load);
	}

	/*
	 * Avg load of busiest sg can be less and avg load of local sg can
	 * be greater than avg load across all sgs of sd because avg load
	 * factors in sg capacity and sgs with smaller group_type are
	 * skipped when updating the busiest sg:
	 */
	if (busiest->group_type != group_misfit_task &&
	    (busiest->avg_load <= sds->avg_load ||
	     local->avg_load >= sds->avg_load)) {
		env->imbalance = 0;
		return fix_small_imbalance(env, sds);
	}

	/*
	 * If there aren't any idle CPUs, avoid creating some.
	 */
	if (busiest->group_type == group_overloaded &&
	    local->group_type   == group_overloaded) {
		load_above_capacity = busiest->sum_nr_running * SCHED_CAPACITY_SCALE;
		if (load_above_capacity > busiest->group_capacity) {
			load_above_capacity -= busiest->group_capacity;
			load_above_capacity *= scale_load_down(NICE_0_LOAD);
			load_above_capacity /= busiest->group_capacity;
		} else
			load_above_capacity = ~0UL;
	}

	/*
	 * We're trying to get all the CPUs to the average_load, so we don't
	 * want to push ourselves above the average load, nor do we wish to
	 * reduce the max loaded CPU below the average load. At the same time,
	 * we also don't want to reduce the group load below the group
	 * capacity. Thus we look for the minimum possible imbalance.
	 */
	max_pull = min(busiest->avg_load - sds->avg_load, load_above_capacity);

	/* How much load to actually move to equalise the imbalance */
	env->imbalance = min(
		max_pull * busiest->group_capacity,
		(sds->avg_load - local->avg_load) * local->group_capacity
	) / SCHED_CAPACITY_SCALE;

	/* Boost imbalance to allow misfit task to be balanced. */
	if (busiest->group_type == group_misfit_task) {
		env->imbalance = max_t(long, env->imbalance,
				       busiest->group_misfit_task_load);
	}

	/*
	 * if *imbalance is less than the average load per runnable task
	 * there is no guarantee that any tasks will be moved so we'll have
	 * a think about bumping its value to force at least one task to be
	 * moved
	 */
	if (env->imbalance < busiest->load_per_task)
		return fix_small_imbalance(env, sds);
}
