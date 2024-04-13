void __blk_mq_tag_idle(struct blk_mq_hw_ctx *hctx)
{
	struct blk_mq_tags *tags = hctx->tags;

	if (!test_and_clear_bit(BLK_MQ_S_TAG_ACTIVE, &hctx->state))
		return;

	atomic_dec(&tags->active_queues);

	blk_mq_tag_wakeup_all(tags, false);
}
