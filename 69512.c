bool blk_mq_has_free_tags(struct blk_mq_tags *tags)
{
	if (!tags)
		return true;

	return bt_has_free_tags(&tags->bitmap_tags);
}
