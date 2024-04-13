void blk_mq_tag_busy_iter(struct blk_mq_hw_ctx *hctx, busy_iter_fn *fn,
		void *priv)
{
	struct blk_mq_tags *tags = hctx->tags;

	if (tags->nr_reserved_tags)
		bt_for_each(hctx, &tags->breserved_tags, 0, fn, priv, true);
	bt_for_each(hctx, &tags->bitmap_tags, tags->nr_reserved_tags, fn, priv,
			false);
}
