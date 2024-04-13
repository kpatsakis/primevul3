static inline int cfs_rq_throttled(struct cfs_rq *cfs_rq)
{
	return cfs_bandwidth_used() && cfs_rq->throttled;
}
