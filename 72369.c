static long calc_group_runnable(struct cfs_rq *cfs_rq, long shares)
{
	long runnable, load_avg;

	load_avg = max(cfs_rq->avg.load_avg,
		       scale_load_down(cfs_rq->load.weight));

	runnable = max(cfs_rq->avg.runnable_load_avg,
		       scale_load_down(cfs_rq->runnable_weight));

	runnable *= shares;
	if (load_avg)
		runnable /= load_avg;

	return clamp_t(long, runnable, MIN_SHARES, shares);
}
