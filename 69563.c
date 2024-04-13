void blk_mq_freeze_queue(struct request_queue *q)
{
	blk_mq_freeze_queue_start(q);
	blk_mq_freeze_queue_wait(q);
}
