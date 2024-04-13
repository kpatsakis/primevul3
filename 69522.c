static void bt_clear_tag(struct blk_mq_bitmap_tags *bt, unsigned int tag)
{
	const int index = TAG_TO_INDEX(bt, tag);
	struct bt_wait_state *bs;
	int wait_cnt;

	clear_bit(TAG_TO_BIT(bt, tag), &bt->map[index].word);

	/* Ensure that the wait list checks occur after clear_bit(). */
	smp_mb();

	bs = bt_wake_ptr(bt);
	if (!bs)
		return;

	wait_cnt = atomic_dec_return(&bs->wait_cnt);
	if (unlikely(wait_cnt < 0))
		wait_cnt = atomic_inc_return(&bs->wait_cnt);
	if (wait_cnt == 0) {
		atomic_add(bt->wake_cnt, &bs->wait_cnt);
		bt_index_atomic_inc(&bt->wake_index);
		wake_up(&bs->wait);
	}
}
