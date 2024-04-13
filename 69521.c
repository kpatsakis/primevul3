static int bt_alloc(struct blk_mq_bitmap_tags *bt, unsigned int depth,
			int node, bool reserved)
{
	int i;

	bt->bits_per_word = ilog2(BITS_PER_LONG);

	/*
	 * Depth can be zero for reserved tags, that's not a failure
	 * condition.
	 */
	if (depth) {
		unsigned int nr, tags_per_word;

		tags_per_word = (1 << bt->bits_per_word);

		/*
		 * If the tag space is small, shrink the number of tags
		 * per word so we spread over a few cachelines, at least.
		 * If less than 4 tags, just forget about it, it's not
		 * going to work optimally anyway.
		 */
		if (depth >= 4) {
			while (tags_per_word * 4 > depth) {
				bt->bits_per_word--;
				tags_per_word = (1 << bt->bits_per_word);
			}
		}

		nr = ALIGN(depth, tags_per_word) / tags_per_word;
		bt->map = kzalloc_node(nr * sizeof(struct blk_align_bitmap),
						GFP_KERNEL, node);
		if (!bt->map)
			return -ENOMEM;

		bt->map_nr = nr;
	}

	bt->bs = kzalloc(BT_WAIT_QUEUES * sizeof(*bt->bs), GFP_KERNEL);
	if (!bt->bs) {
		kfree(bt->map);
		bt->map = NULL;
		return -ENOMEM;
	}

	bt_update_count(bt, depth);

	for (i = 0; i < BT_WAIT_QUEUES; i++) {
		init_waitqueue_head(&bt->bs[i].wait);
		atomic_set(&bt->bs[i].wait_cnt, bt->wake_cnt);
	}

	return 0;
}
