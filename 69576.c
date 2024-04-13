static struct blk_mq_tags *blk_mq_init_rq_map(struct blk_mq_tag_set *set,
		unsigned int hctx_idx)
{
	struct blk_mq_tags *tags;
	unsigned int i, j, entries_per_page, max_order = 4;
	size_t rq_size, left;

	tags = blk_mq_init_tags(set->queue_depth, set->reserved_tags,
				set->numa_node,
				BLK_MQ_FLAG_TO_ALLOC_POLICY(set->flags));
	if (!tags)
		return NULL;

	INIT_LIST_HEAD(&tags->page_list);

	tags->rqs = kzalloc_node(set->queue_depth * sizeof(struct request *),
				 GFP_KERNEL | __GFP_NOWARN | __GFP_NORETRY,
				 set->numa_node);
	if (!tags->rqs) {
		blk_mq_free_tags(tags);
		return NULL;
	}

	/*
	 * rq_size is the size of the request plus driver payload, rounded
	 * to the cacheline size
	 */
	rq_size = round_up(sizeof(struct request) + set->cmd_size,
				cache_line_size());
	left = rq_size * set->queue_depth;

	for (i = 0; i < set->queue_depth; ) {
		int this_order = max_order;
		struct page *page;
		int to_do;
		void *p;

		while (left < order_to_size(this_order - 1) && this_order)
			this_order--;

		do {
			page = alloc_pages_node(set->numa_node,
				GFP_KERNEL | __GFP_NOWARN | __GFP_NORETRY | __GFP_ZERO,
				this_order);
			if (page)
				break;
			if (!this_order--)
				break;
			if (order_to_size(this_order) < rq_size)
				break;
		} while (1);

		if (!page)
			goto fail;

		page->private = this_order;
		list_add_tail(&page->lru, &tags->page_list);

		p = page_address(page);
		entries_per_page = order_to_size(this_order) / rq_size;
		to_do = min(entries_per_page, set->queue_depth - i);
		left -= to_do * rq_size;
		for (j = 0; j < to_do; j++) {
			tags->rqs[i] = p;
			if (set->ops->init_request) {
				if (set->ops->init_request(set->driver_data,
						tags->rqs[i], hctx_idx, i,
						set->numa_node)) {
					tags->rqs[i] = NULL;
					goto fail;
				}
			}

			p += rq_size;
			i++;
		}
	}
	return tags;

fail:
	blk_mq_free_rq_map(set, tags, hctx_idx);
	return NULL;
}
