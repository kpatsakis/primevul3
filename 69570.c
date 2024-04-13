static int blk_mq_hctx_notify(void *data, unsigned long action,
			      unsigned int cpu)
{
	struct blk_mq_hw_ctx *hctx = data;

	if (action == CPU_DEAD || action == CPU_DEAD_FROZEN)
		return blk_mq_hctx_cpu_offline(hctx, cpu);

	/*
	 * In case of CPU online, tags may be reallocated
	 * in blk_mq_map_swqueue() after mapping is updated.
	 */

	return NOTIFY_OK;
}
