void blk_mq_run_hw_queue(struct blk_mq_hw_ctx *hctx, bool async)
{
	if (unlikely(test_bit(BLK_MQ_S_STOPPED, &hctx->state) ||
	    !blk_mq_hw_queue_mapped(hctx)))
		return;

	if (!async) {
		int cpu = get_cpu();
		if (cpumask_test_cpu(cpu, hctx->cpumask)) {
			__blk_mq_run_hw_queue(hctx);
			put_cpu();
			return;
		}

		put_cpu();
	}

	kblockd_schedule_delayed_work_on(blk_mq_hctx_next_cpu(hctx),
			&hctx->run_work, 0);
}
