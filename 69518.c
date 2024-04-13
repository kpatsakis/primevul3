ssize_t blk_mq_tag_sysfs_show(struct blk_mq_tags *tags, char *page)
{
	char *orig_page = page;
	unsigned int free, res;

	if (!tags)
		return 0;

	page += sprintf(page, "nr_tags=%u, reserved_tags=%u, "
			"bits_per_word=%u\n",
			tags->nr_tags, tags->nr_reserved_tags,
			tags->bitmap_tags.bits_per_word);

	free = bt_unused_tags(&tags->bitmap_tags);
	res = bt_unused_tags(&tags->breserved_tags);

	page += sprintf(page, "nr_free=%u, nr_reserved=%u\n", free, res);
	page += sprintf(page, "active_queues=%u\n", atomic_read(&tags->active_queues));

	return page - orig_page;
}
