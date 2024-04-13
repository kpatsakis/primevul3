static void attach_task_cfs_rq(struct task_struct *p)
{
	struct sched_entity *se = &p->se;
	struct cfs_rq *cfs_rq = cfs_rq_of(se);

	attach_entity_cfs_rq(se);

	if (!vruntime_normalized(p))
		se->vruntime += cfs_rq->min_vruntime;
}
