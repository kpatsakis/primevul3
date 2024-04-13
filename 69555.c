static void blk_mq_exit_hw_queues(struct request_queue *q,
		struct blk_mq_tag_set *set, int nr_queue)
{
	struct blk_mq_hw_ctx *hctx;
	unsigned int i;

	queue_for_each_hw_ctx(q, hctx, i) {
		if (i == nr_queue)
			break;
		blk_mq_exit_hctx(q, set, hctx, i);
	}
}
