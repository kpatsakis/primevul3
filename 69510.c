void blk_mq_all_tag_busy_iter(struct blk_mq_tags *tags, busy_tag_iter_fn *fn,
		void *priv)
{
	if (tags->nr_reserved_tags)
		bt_tags_for_each(tags, &tags->breserved_tags, 0, fn, priv, true);
	bt_tags_for_each(tags, &tags->bitmap_tags, tags->nr_reserved_tags, fn, priv,
			false);
}
