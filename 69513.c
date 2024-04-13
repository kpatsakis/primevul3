static struct blk_mq_tags *blk_mq_init_bitmap_tags(struct blk_mq_tags *tags,
						   int node, int alloc_policy)
{
	unsigned int depth = tags->nr_tags - tags->nr_reserved_tags;

	tags->alloc_policy = alloc_policy;

	if (bt_alloc(&tags->bitmap_tags, depth, node, false))
		goto enomem;
	if (bt_alloc(&tags->breserved_tags, tags->nr_reserved_tags, node, true))
		goto enomem;

	return tags;
enomem:
	bt_free(&tags->bitmap_tags);
	kfree(tags);
	return NULL;
}
