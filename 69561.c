static void blk_mq_free_rq_map(struct blk_mq_tag_set *set,
		struct blk_mq_tags *tags, unsigned int hctx_idx)
{
	struct page *page;

	if (tags->rqs && set->ops->exit_request) {
		int i;

		for (i = 0; i < tags->nr_tags; i++) {
			if (!tags->rqs[i])
				continue;
			set->ops->exit_request(set->driver_data, tags->rqs[i],
						hctx_idx, i);
			tags->rqs[i] = NULL;
		}
	}

	while (!list_empty(&tags->page_list)) {
		page = list_first_entry(&tags->page_list, struct page, lru);
		list_del_init(&page->lru);
		__free_pages(page, page->private);
	}

	kfree(tags->rqs);

	blk_mq_free_tags(tags);
}
