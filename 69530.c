static struct bt_wait_state *bt_wait_ptr(struct blk_mq_bitmap_tags *bt,
					 struct blk_mq_hw_ctx *hctx)
{
	struct bt_wait_state *bs;
	int wait_index;

	if (!hctx)
		return &bt->bs[0];

	wait_index = atomic_read(&hctx->wait_index);
	bs = &bt->bs[wait_index];
	bt_index_atomic_inc(&hctx->wait_index);
	return bs;
}
