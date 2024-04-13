static unsigned int __blk_mq_get_tag(struct blk_mq_alloc_data *data)
{
	int tag;

	tag = bt_get(data, &data->hctx->tags->bitmap_tags, data->hctx,
			&data->ctx->last_tag, data->hctx->tags);
	if (tag >= 0)
		return tag + data->hctx->tags->nr_reserved_tags;

	return BLK_MQ_TAG_FAIL;
}
