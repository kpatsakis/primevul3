static unsigned int __blk_mq_get_reserved_tag(struct blk_mq_alloc_data *data)
{
	int tag, zero = 0;

	if (unlikely(!data->hctx->tags->nr_reserved_tags)) {
		WARN_ON_ONCE(1);
		return BLK_MQ_TAG_FAIL;
	}

	tag = bt_get(data, &data->hctx->tags->breserved_tags, NULL, &zero,
		data->hctx->tags);
	if (tag < 0)
		return BLK_MQ_TAG_FAIL;

	return tag;
}
