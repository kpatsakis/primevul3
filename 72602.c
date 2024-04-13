static inline int throttled_hierarchy(struct cfs_rq *cfs_rq)
{
	return cfs_bandwidth_used() && cfs_rq->throttle_count;
}
