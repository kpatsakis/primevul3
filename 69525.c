static bool bt_has_free_tags(struct blk_mq_bitmap_tags *bt)
{
	int i;

	for (i = 0; i < bt->map_nr; i++) {
		struct blk_align_bitmap *bm = &bt->map[i];
		int ret;

		ret = find_first_zero_bit(&bm->word, bm->depth);
		if (ret < bm->depth)
			return true;
	}

	return false;
}
