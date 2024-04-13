enqueue_runnable_load_avg(struct cfs_rq *cfs_rq, struct sched_entity *se)
{
	cfs_rq->runnable_weight += se->runnable_weight;

	cfs_rq->avg.runnable_load_avg += se->avg.runnable_load_avg;
	cfs_rq->avg.runnable_load_sum += se_runnable(se) * se->avg.runnable_load_sum;
}
