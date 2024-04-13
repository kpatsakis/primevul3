static int blk_mq_hctx_next_cpu(struct blk_mq_hw_ctx *hctx)
{
	if (hctx->queue->nr_hw_queues == 1)
		return WORK_CPU_UNBOUND;

	if (--hctx->next_cpu_batch <= 0) {
		int cpu = hctx->next_cpu, next_cpu;

		next_cpu = cpumask_next(hctx->next_cpu, hctx->cpumask);
		if (next_cpu >= nr_cpu_ids)
			next_cpu = cpumask_first(hctx->cpumask);

		hctx->next_cpu = next_cpu;
		hctx->next_cpu_batch = BLK_MQ_CPU_WORK_BATCH;

		return cpu;
	}

	return hctx->next_cpu;
}
