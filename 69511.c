unsigned int blk_mq_get_tag(struct blk_mq_alloc_data *data)
{
	if (!data->reserved)
		return __blk_mq_get_tag(data);

	return __blk_mq_get_reserved_tag(data);
}
