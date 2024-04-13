void cpu_load_update_active(struct rq *this_rq)
{
	unsigned long load = weighted_cpuload(this_rq);

	if (tick_nohz_tick_stopped())
		cpu_load_update_nohz(this_rq, READ_ONCE(jiffies), load);
	else
		cpu_load_update_periodic(this_rq, load);
}
