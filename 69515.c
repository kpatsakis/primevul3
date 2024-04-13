void blk_mq_put_tag(struct blk_mq_hw_ctx *hctx, unsigned int tag,
		    unsigned int *last_tag)
{
	struct blk_mq_tags *tags = hctx->tags;

	if (tag >= tags->nr_reserved_tags) {
		const int real_tag = tag - tags->nr_reserved_tags;

		BUG_ON(real_tag >= tags->nr_tags);
		bt_clear_tag(&tags->bitmap_tags, real_tag);
		if (likely(tags->alloc_policy == BLK_TAG_ALLOC_FIFO))
			*last_tag = real_tag;
	} else {
		BUG_ON(tag >= tags->nr_reserved_tags);
		bt_clear_tag(&tags->breserved_tags, tag);
	}
}
