static int idle_balance(struct rq *this_rq, struct rq_flags *rf)
{
	unsigned long next_balance = jiffies + HZ;
	int this_cpu = this_rq->cpu;
	struct sched_domain *sd;
	int pulled_task = 0;
	u64 curr_cost = 0;

	/*
	 * We must set idle_stamp _before_ calling idle_balance(), such that we
	 * measure the duration of idle_balance() as idle time.
	 */
	this_rq->idle_stamp = rq_clock(this_rq);

	/*
	 * Do not pull tasks towards !active CPUs...
	 */
	if (!cpu_active(this_cpu))
		return 0;

	/*
	 * This is OK, because current is on_cpu, which avoids it being picked
	 * for load-balance and preemption/IRQs are still disabled avoiding
	 * further scheduler activity on it and we're being very careful to
	 * re-start the picking loop.
	 */
	rq_unpin_lock(this_rq, rf);

	if (this_rq->avg_idle < sysctl_sched_migration_cost ||
	    !READ_ONCE(this_rq->rd->overload)) {

		rcu_read_lock();
		sd = rcu_dereference_check_sched_domain(this_rq->sd);
		if (sd)
			update_next_balance(sd, &next_balance);
		rcu_read_unlock();

		nohz_newidle_balance(this_rq);

		goto out;
	}

	raw_spin_unlock(&this_rq->lock);

	update_blocked_averages(this_cpu);
	rcu_read_lock();
	for_each_domain(this_cpu, sd) {
		int continue_balancing = 1;
		u64 t0, domain_cost;

		if (!(sd->flags & SD_LOAD_BALANCE))
			continue;

		if (this_rq->avg_idle < curr_cost + sd->max_newidle_lb_cost) {
			update_next_balance(sd, &next_balance);
			break;
		}

		if (sd->flags & SD_BALANCE_NEWIDLE) {
			t0 = sched_clock_cpu(this_cpu);

			pulled_task = load_balance(this_cpu, this_rq,
						   sd, CPU_NEWLY_IDLE,
						   &continue_balancing);

			domain_cost = sched_clock_cpu(this_cpu) - t0;
			if (domain_cost > sd->max_newidle_lb_cost)
				sd->max_newidle_lb_cost = domain_cost;

			curr_cost += domain_cost;
		}

		update_next_balance(sd, &next_balance);

		/*
		 * Stop searching for tasks to pull if there are
		 * now runnable tasks on this rq.
		 */
		if (pulled_task || this_rq->nr_running > 0)
			break;
	}
	rcu_read_unlock();

	raw_spin_lock(&this_rq->lock);

	if (curr_cost > this_rq->max_idle_balance_cost)
		this_rq->max_idle_balance_cost = curr_cost;

out:
	/*
	 * While browsing the domains, we released the rq lock, a task could
	 * have been enqueued in the meantime. Since we're not going idle,
	 * pretend we pulled a task.
	 */
	if (this_rq->cfs.h_nr_running && !pulled_task)
		pulled_task = 1;

	/* Move the next balance forward */
	if (time_after(this_rq->next_balance, next_balance))
		this_rq->next_balance = next_balance;

	/* Is there a task of a high priority class? */
	if (this_rq->nr_running != this_rq->cfs.h_nr_running)
		pulled_task = -1;

	if (pulled_task)
		this_rq->idle_stamp = 0;

	rq_repin_lock(this_rq, rf);

	return pulled_task;
}
