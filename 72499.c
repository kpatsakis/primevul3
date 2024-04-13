static void nohz_balancer_kick(struct rq *rq)
{
	unsigned long now = jiffies;
	struct sched_domain_shared *sds;
	struct sched_domain *sd;
	int nr_busy, i, cpu = rq->cpu;
	unsigned int flags = 0;

	if (unlikely(rq->idle_balance))
		return;

	/*
	 * We may be recently in ticked or tickless idle mode. At the first
	 * busy tick after returning from idle, we will update the busy stats.
	 */
	nohz_balance_exit_idle(rq);

	/*
	 * None are in tickless mode and hence no need for NOHZ idle load
	 * balancing.
	 */
	if (likely(!atomic_read(&nohz.nr_cpus)))
		return;

	if (READ_ONCE(nohz.has_blocked) &&
	    time_after(now, READ_ONCE(nohz.next_blocked)))
		flags = NOHZ_STATS_KICK;

	if (time_before(now, nohz.next_balance))
		goto out;

	if (rq->nr_running >= 2 || rq->misfit_task_load) {
		flags = NOHZ_KICK_MASK;
		goto out;
	}

	rcu_read_lock();
	sds = rcu_dereference(per_cpu(sd_llc_shared, cpu));
	if (sds) {
		/*
		 * XXX: write a coherent comment on why we do this.
		 * See also: http://lkml.kernel.org/r/20111202010832.602203411@sbsiddha-desk.sc.intel.com
		 */
		nr_busy = atomic_read(&sds->nr_busy_cpus);
		if (nr_busy > 1) {
			flags = NOHZ_KICK_MASK;
			goto unlock;
		}

	}

	sd = rcu_dereference(rq->sd);
	if (sd) {
		if ((rq->cfs.h_nr_running >= 1) &&
				check_cpu_capacity(rq, sd)) {
			flags = NOHZ_KICK_MASK;
			goto unlock;
		}
	}

	sd = rcu_dereference(per_cpu(sd_asym_packing, cpu));
	if (sd) {
		for_each_cpu(i, sched_domain_span(sd)) {
			if (i == cpu ||
			    !cpumask_test_cpu(i, nohz.idle_cpus_mask))
				continue;

			if (sched_asym_prefer(i, cpu)) {
				flags = NOHZ_KICK_MASK;
				goto unlock;
			}
		}
	}
unlock:
	rcu_read_unlock();
out:
	if (flags)
		kick_ilb(flags);
}
