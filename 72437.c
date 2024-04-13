enqueue_task_fair(struct rq *rq, struct task_struct *p, int flags)
{
	struct cfs_rq *cfs_rq;
	struct sched_entity *se = &p->se;

	/*
	 * The code below (indirectly) updates schedutil which looks at
	 * the cfs_rq utilization to select a frequency.
	 * Let's add the task's estimated utilization to the cfs_rq's
	 * estimated utilization, before we update schedutil.
	 */
	util_est_enqueue(&rq->cfs, p);

	/*
	 * If in_iowait is set, the code below may not trigger any cpufreq
	 * utilization updates, so do it here explicitly with the IOWAIT flag
	 * passed.
	 */
	if (p->in_iowait)
		cpufreq_update_util(rq, SCHED_CPUFREQ_IOWAIT);

	for_each_sched_entity(se) {
		if (se->on_rq)
			break;
		cfs_rq = cfs_rq_of(se);
		enqueue_entity(cfs_rq, se, flags);

		/*
		 * end evaluation on encountering a throttled cfs_rq
		 *
		 * note: in the case of encountering a throttled cfs_rq we will
		 * post the final h_nr_running increment below.
		 */
		if (cfs_rq_throttled(cfs_rq))
			break;
		cfs_rq->h_nr_running++;

		flags = ENQUEUE_WAKEUP;
	}

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_of(se);
		cfs_rq->h_nr_running++;

		if (cfs_rq_throttled(cfs_rq))
			break;

		update_load_avg(cfs_rq, se, UPDATE_TG);
		update_cfs_group(se);
	}

	if (!se) {
		add_nr_running(rq, 1);
		/*
		 * Since new tasks are assigned an initial util_avg equal to
		 * half of the spare capacity of their CPU, tiny tasks have the
		 * ability to cross the overutilized threshold, which will
		 * result in the load balancer ruining all the task placement
		 * done by EAS. As a way to mitigate that effect, do not account
		 * for the first enqueue operation of new tasks during the
		 * overutilized flag detection.
		 *
		 * A better way of solving this problem would be to wait for
		 * the PELT signals of tasks to converge before taking them
		 * into account, but that is not straightforward to implement,
		 * and the following generally works well enough in practice.
		 */
		if (flags & ENQUEUE_WAKEUP)
			update_overutilized_status(rq);

	}

	hrtick_update(rq);
}
