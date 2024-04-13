static void cpu_load_update_periodic(struct rq *this_rq, unsigned long load)
{
#ifdef CONFIG_NO_HZ_COMMON
	/* See the mess around cpu_load_update_nohz(). */
	this_rq->last_load_update_tick = READ_ONCE(jiffies);
#endif
	cpu_load_update(this_rq, load, 1);
}
