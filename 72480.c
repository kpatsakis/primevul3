void init_entity_runnable_average(struct sched_entity *se)
{
	struct sched_avg *sa = &se->avg;

	memset(sa, 0, sizeof(*sa));

	/*
	 * Tasks are initialized with full load to be seen as heavy tasks until
	 * they get a chance to stabilize to their real load level.
	 * Group entities are initialized with zero load to reflect the fact that
	 * nothing has been attached to the task group yet.
	 */
	if (entity_is_task(se))
		sa->runnable_load_avg = sa->load_avg = scale_load_down(se->load.weight);

	se->runnable_weight = se->load.weight;

	/* when this task enqueue'ed, it will contribute to its cfs_rq's load_avg */
}
