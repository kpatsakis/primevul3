static unsigned long weighted_cpuload(struct rq *rq)
{
	return cfs_rq_runnable_load_avg(&rq->cfs);
}
