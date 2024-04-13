static void propagate_entity_cfs_rq(struct sched_entity *se)
{
	struct cfs_rq *cfs_rq;

	/* Start to propagate at parent */
	se = se->parent;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_of(se);

		if (cfs_rq_throttled(cfs_rq))
			break;

		update_load_avg(cfs_rq, se, UPDATE_TG);
	}
}
