static void update_cfs_group(struct sched_entity *se)
{
	struct cfs_rq *gcfs_rq = group_cfs_rq(se);
	long shares, runnable;

	if (!gcfs_rq)
		return;

	if (throttled_hierarchy(gcfs_rq))
		return;

#ifndef CONFIG_SMP
	runnable = shares = READ_ONCE(gcfs_rq->tg->shares);

	if (likely(se->load.weight == shares))
		return;
#else
	shares   = calc_group_shares(gcfs_rq);
	runnable = calc_group_runnable(gcfs_rq, shares);
#endif

	reweight_entity(cfs_rq_of(se), se, shares, runnable);
}
