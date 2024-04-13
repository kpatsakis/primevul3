struct blk_mq_hw_ctx *blk_mq_map_queue(struct request_queue *q, const int cpu)
{
	return q->queue_hw_ctx[q->mq_map[cpu]];
}
