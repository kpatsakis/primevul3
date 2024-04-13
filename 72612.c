static inline void update_blocked_averages(int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	struct cfs_rq *cfs_rq = &rq->cfs;
	const struct sched_class *curr_class;
	struct rq_flags rf;

	rq_lock_irqsave(rq, &rf);
	update_rq_clock(rq);
	update_cfs_rq_load_avg(cfs_rq_clock_task(cfs_rq), cfs_rq);

	curr_class = rq->curr->sched_class;
	update_rt_rq_load_avg(rq_clock_task(rq), rq, curr_class == &rt_sched_class);
	update_dl_rq_load_avg(rq_clock_task(rq), rq, curr_class == &dl_sched_class);
	update_irq_load_avg(rq, 0);
#ifdef CONFIG_NO_HZ_COMMON
	rq->last_blocked_load_update_tick = jiffies;
	if (!cfs_rq_has_blocked(cfs_rq) && !others_have_blocked(rq))
		rq->has_blocked_load = 0;
#endif
	rq_unlock_irqrestore(rq, &rf);
}
