static void migrate_task_rq_fair(struct task_struct *p, int new_cpu)
{
	/*
	 * As blocked tasks retain absolute vruntime the migration needs to
	 * deal with this by subtracting the old and adding the new
	 * min_vruntime -- the latter is done by enqueue_entity() when placing
	 * the task on the new runqueue.
	 */
	if (p->state == TASK_WAKING) {
		struct sched_entity *se = &p->se;
		struct cfs_rq *cfs_rq = cfs_rq_of(se);
		u64 min_vruntime;

#ifndef CONFIG_64BIT
		u64 min_vruntime_copy;

		do {
			min_vruntime_copy = cfs_rq->min_vruntime_copy;
			smp_rmb();
			min_vruntime = cfs_rq->min_vruntime;
		} while (min_vruntime != min_vruntime_copy);
#else
		min_vruntime = cfs_rq->min_vruntime;
#endif

		se->vruntime -= min_vruntime;
	}

	if (p->on_rq == TASK_ON_RQ_MIGRATING) {
		/*
		 * In case of TASK_ON_RQ_MIGRATING we in fact hold the 'old'
		 * rq->lock and can modify state directly.
		 */
		lockdep_assert_held(&task_rq(p)->lock);
		detach_entity_cfs_rq(&p->se);

	} else {
		/*
		 * We are supposed to update the task to "current" time, then
		 * its up to date and ready to go to new CPU/cfs_rq. But we
		 * have difficulty in getting what current time is, so simply
		 * throw away the out-of-date time. This will result in the
		 * wakee task is less decayed, but giving the wakee more load
		 * sounds not bad.
		 */
		remove_entity_load_avg(&p->se);
	}

	/* Tell new CPU we are migrated */
	p->se.avg.last_update_time = 0;

	/* We have migrated, no longer consider this task hot */
	p->se.exec_start = 0;

	update_scan_period(p, new_cpu);
}
