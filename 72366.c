static void attach_task(struct rq *rq, struct task_struct *p)
{
	lockdep_assert_held(&rq->lock);

	BUG_ON(task_rq(p) != rq);
	activate_task(rq, p, ENQUEUE_NOCLOCK);
	p->on_rq = TASK_ON_RQ_QUEUED;
	check_preempt_curr(rq, p, 0);
}
