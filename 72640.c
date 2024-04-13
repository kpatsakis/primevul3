update_tg_cfs_runnable(struct cfs_rq *cfs_rq, struct sched_entity *se, struct cfs_rq *gcfs_rq)
{
	long delta_avg, running_sum, runnable_sum = gcfs_rq->prop_runnable_sum;
	unsigned long runnable_load_avg, load_avg;
	u64 runnable_load_sum, load_sum = 0;
	s64 delta_sum;

	if (!runnable_sum)
		return;

	gcfs_rq->prop_runnable_sum = 0;

	if (runnable_sum >= 0) {
		/*
		 * Add runnable; clip at LOAD_AVG_MAX. Reflects that until
		 * the CPU is saturated running == runnable.
		 */
		runnable_sum += se->avg.load_sum;
		runnable_sum = min(runnable_sum, (long)LOAD_AVG_MAX);
	} else {
		/*
		 * Estimate the new unweighted runnable_sum of the gcfs_rq by
		 * assuming all tasks are equally runnable.
		 */
		if (scale_load_down(gcfs_rq->load.weight)) {
			load_sum = div_s64(gcfs_rq->avg.load_sum,
				scale_load_down(gcfs_rq->load.weight));
		}

		/* But make sure to not inflate se's runnable */
		runnable_sum = min(se->avg.load_sum, load_sum);
	}

	/*
	 * runnable_sum can't be lower than running_sum
	 * As running sum is scale with CPU capacity wehreas the runnable sum
	 * is not we rescale running_sum 1st
	 */
	running_sum = se->avg.util_sum /
		arch_scale_cpu_capacity(NULL, cpu_of(rq_of(cfs_rq)));
	runnable_sum = max(runnable_sum, running_sum);

	load_sum = (s64)se_weight(se) * runnable_sum;
	load_avg = div_s64(load_sum, LOAD_AVG_MAX);

	delta_sum = load_sum - (s64)se_weight(se) * se->avg.load_sum;
	delta_avg = load_avg - se->avg.load_avg;

	se->avg.load_sum = runnable_sum;
	se->avg.load_avg = load_avg;
	add_positive(&cfs_rq->avg.load_avg, delta_avg);
	add_positive(&cfs_rq->avg.load_sum, delta_sum);

	runnable_load_sum = (s64)se_runnable(se) * runnable_sum;
	runnable_load_avg = div_s64(runnable_load_sum, LOAD_AVG_MAX);
	delta_sum = runnable_load_sum - se_weight(se) * se->avg.runnable_load_sum;
	delta_avg = runnable_load_avg - se->avg.runnable_load_avg;

	se->avg.runnable_load_sum = runnable_sum;
	se->avg.runnable_load_avg = runnable_load_avg;

	if (se->on_rq) {
		add_positive(&cfs_rq->avg.runnable_load_avg, delta_avg);
		add_positive(&cfs_rq->avg.runnable_load_sum, delta_sum);
	}
}
