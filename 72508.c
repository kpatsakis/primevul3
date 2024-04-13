static inline int on_null_domain(struct rq *rq)
{
	return unlikely(!rcu_dereference_sched(rq->sd));
}
