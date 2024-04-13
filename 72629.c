static inline void update_overutilized_status(struct rq *rq)
{
	if (!READ_ONCE(rq->rd->overutilized) && cpu_overutilized(rq->cpu))
		WRITE_ONCE(rq->rd->overutilized, SG_OVERUTILIZED);
}
