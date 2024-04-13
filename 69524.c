static int bt_get(struct blk_mq_alloc_data *data,
		struct blk_mq_bitmap_tags *bt,
		struct blk_mq_hw_ctx *hctx,
		unsigned int *last_tag, struct blk_mq_tags *tags)
{
	struct bt_wait_state *bs;
	DEFINE_WAIT(wait);
	int tag;

	tag = __bt_get(hctx, bt, last_tag, tags);
	if (tag != -1)
		return tag;

	if (!(data->gfp & __GFP_WAIT))
		return -1;

	bs = bt_wait_ptr(bt, hctx);
	do {
		prepare_to_wait(&bs->wait, &wait, TASK_UNINTERRUPTIBLE);

		tag = __bt_get(hctx, bt, last_tag, tags);
		if (tag != -1)
			break;

		/*
		 * We're out of tags on this hardware queue, kick any
		 * pending IO submits before going to sleep waiting for
		 * some to complete. Note that hctx can be NULL here for
		 * reserved tag allocation.
		 */
		if (hctx)
			blk_mq_run_hw_queue(hctx, false);

		/*
		 * Retry tag allocation after running the hardware queue,
		 * as running the queue may also have found completions.
		 */
		tag = __bt_get(hctx, bt, last_tag, tags);
		if (tag != -1)
			break;

		blk_mq_put_ctx(data->ctx);

		io_schedule();

		data->ctx = blk_mq_get_ctx(data->q);
		data->hctx = data->q->mq_ops->map_queue(data->q,
				data->ctx->cpu);
		if (data->reserved) {
			bt = &data->hctx->tags->breserved_tags;
		} else {
			last_tag = &data->ctx->last_tag;
			hctx = data->hctx;
			bt = &hctx->tags->bitmap_tags;
		}
		finish_wait(&bs->wait, &wait);
		bs = bt_wait_ptr(bt, hctx);
	} while (1);

	finish_wait(&bs->wait, &wait);
	return tag;
}
