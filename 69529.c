static void bt_update_count(struct blk_mq_bitmap_tags *bt,
			    unsigned int depth)
{
	unsigned int tags_per_word = 1U << bt->bits_per_word;
	unsigned int map_depth = depth;

	if (depth) {
		int i;

		for (i = 0; i < bt->map_nr; i++) {
			bt->map[i].depth = min(map_depth, tags_per_word);
			map_depth -= bt->map[i].depth;
		}
	}

	bt->wake_cnt = BT_WAIT_BATCH;
	if (bt->wake_cnt > depth / BT_WAIT_QUEUES)
		bt->wake_cnt = max(1U, depth / BT_WAIT_QUEUES);

	bt->depth = depth;
}
