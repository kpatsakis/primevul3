static void blk_mq_exit_hctx(struct request_queue *q,
		struct blk_mq_tag_set *set,
		struct blk_mq_hw_ctx *hctx, unsigned int hctx_idx)
{
	unsigned flush_start_tag = set->queue_depth;

	blk_mq_tag_idle(hctx);

	if (set->ops->exit_request)
		set->ops->exit_request(set->driver_data,
				       hctx->fq->flush_rq, hctx_idx,
				       flush_start_tag + hctx_idx);

	if (set->ops->exit_hctx)
		set->ops->exit_hctx(hctx, hctx_idx);

	blk_mq_unregister_cpu_notifier(&hctx->cpu_notifier);
	blk_free_flush_queue(hctx->fq);
	blk_mq_free_bitmap(&hctx->ctx_map);
}
