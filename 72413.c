static unsigned long cpu_util_next(int cpu, struct task_struct *p, int dst_cpu)
{
	struct cfs_rq *cfs_rq = &cpu_rq(cpu)->cfs;
	unsigned long util_est, util = READ_ONCE(cfs_rq->avg.util_avg);

	/*
	 * If @p migrates from @cpu to another, remove its contribution. Or,
	 * if @p migrates from another CPU to @cpu, add its contribution. In
	 * the other cases, @cpu is not impacted by the migration, so the
	 * util_avg should already be correct.
	 */
	if (task_cpu(p) == cpu && dst_cpu != cpu)
		sub_positive(&util, task_util(p));
	else if (task_cpu(p) != cpu && dst_cpu == cpu)
		util += task_util(p);

	if (sched_feat(UTIL_EST)) {
		util_est = READ_ONCE(cfs_rq->avg.util_est.enqueued);

		/*
		 * During wake-up, the task isn't enqueued yet and doesn't
		 * appear in the cfs_rq->avg.util_est.enqueued of any rq,
		 * so just add it (if needed) to "simulate" what will be
		 * cpu_util() after the task has been enqueued.
		 */
		if (dst_cpu == cpu)
			util_est += _task_util_est(p);

		util = max(util, util_est);
	}

	return min(util, capacity_orig_of(cpu));
}
