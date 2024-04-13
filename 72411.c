void cpu_load_update_nohz_stop(void)
{
	unsigned long curr_jiffies = READ_ONCE(jiffies);
	struct rq *this_rq = this_rq();
	unsigned long load;
	struct rq_flags rf;

	if (curr_jiffies == this_rq->last_load_update_tick)
		return;

	load = weighted_cpuload(this_rq);
	rq_lock(this_rq, &rf);
	update_rq_clock(this_rq);
	cpu_load_update_nohz(this_rq, curr_jiffies, load);
	rq_unlock(this_rq, &rf);
}
