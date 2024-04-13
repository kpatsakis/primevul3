void online_fair_sched_group(struct task_group *tg)
{
	struct sched_entity *se;
	struct rq *rq;
	int i;

	for_each_possible_cpu(i) {
		rq = cpu_rq(i);
		se = tg->se[i];

		raw_spin_lock_irq(&rq->lock);
		update_rq_clock(rq);
		attach_entity_cfs_rq(se);
		sync_throttle(tg, i);
		raw_spin_unlock_irq(&rq->lock);
	}
}
