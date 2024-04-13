static int __bt_get(struct blk_mq_hw_ctx *hctx, struct blk_mq_bitmap_tags *bt,
		    unsigned int *tag_cache, struct blk_mq_tags *tags)
{
	unsigned int last_tag, org_last_tag;
	int index, i, tag;

	if (!hctx_may_queue(hctx, bt))
		return -1;

	last_tag = org_last_tag = *tag_cache;
	index = TAG_TO_INDEX(bt, last_tag);

	for (i = 0; i < bt->map_nr; i++) {
		tag = __bt_get_word(&bt->map[index], TAG_TO_BIT(bt, last_tag),
				    BT_ALLOC_RR(tags));
		if (tag != -1) {
			tag += (index << bt->bits_per_word);
			goto done;
		}

		/*
		 * Jump to next index, and reset the last tag to be the
		 * first tag of that index
		 */
		index++;
		last_tag = (index << bt->bits_per_word);

		if (index >= bt->map_nr) {
			index = 0;
			last_tag = 0;
		}
	}

	*tag_cache = 0;
	return -1;

	/*
	 * Only update the cache from the allocation path, if we ended
	 * up using the specific cached tag.
	 */
done:
	if (tag == org_last_tag || unlikely(BT_ALLOC_RR(tags))) {
		last_tag = tag + 1;
		if (last_tag >= bt->depth - 1)
			last_tag = 0;

		*tag_cache = last_tag;
	}

	return tag;
}
