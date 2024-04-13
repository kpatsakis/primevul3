void blk_mq_tag_init_last_tag(struct blk_mq_tags *tags, unsigned int *tag)
{
	unsigned int depth = tags->nr_tags - tags->nr_reserved_tags;

	*tag = prandom_u32() % depth;
}
