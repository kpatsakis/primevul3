static void blk_mq_update_tag_set_depth(struct blk_mq_tag_set *set)
{
	struct blk_mq_hw_ctx *hctx;
	struct request_queue *q;
	bool shared;
	int i;

	if (set->tag_list.next == set->tag_list.prev)
		shared = false;
	else
		shared = true;

	list_for_each_entry(q, &set->tag_list, tag_set_list) {
		blk_mq_freeze_queue(q);

		queue_for_each_hw_ctx(q, hctx, i) {
			if (shared)
				hctx->flags |= BLK_MQ_F_TAG_SHARED;
			else
				hctx->flags &= ~BLK_MQ_F_TAG_SHARED;
		}
		blk_mq_unfreeze_queue(q);
	}
}
