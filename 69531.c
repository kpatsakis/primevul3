static inline bool hctx_may_queue(struct blk_mq_hw_ctx *hctx,
				  struct blk_mq_bitmap_tags *bt)
{
	unsigned int depth, users;

	if (!hctx || !(hctx->flags & BLK_MQ_F_TAG_SHARED))
		return true;
	if (!test_bit(BLK_MQ_S_TAG_ACTIVE, &hctx->state))
		return true;

	/*
	 * Don't try dividing an ant
	 */
	if (bt->depth == 1)
		return true;

	users = atomic_read(&hctx->tags->active_queues);
	if (!users)
		return true;

	/*
	 * Allow at least some tags
	 */
	depth = max((bt->depth + users - 1) / users, 4U);
	return atomic_read(&hctx->nr_active) < depth;
}
