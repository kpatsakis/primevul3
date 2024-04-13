static void flush_busy_ctxs(struct blk_mq_hw_ctx *hctx, struct list_head *list)
{
	struct blk_mq_ctx *ctx;
	int i;

	for (i = 0; i < hctx->ctx_map.size; i++) {
		struct blk_align_bitmap *bm = &hctx->ctx_map.map[i];
		unsigned int off, bit;

		if (!bm->word)
			continue;

		bit = 0;
		off = i * hctx->ctx_map.bits_per_word;
		do {
			bit = find_next_bit(&bm->word, bm->depth, bit);
			if (bit >= bm->depth)
				break;

			ctx = hctx->ctxs[bit + off];
			clear_bit(bit, &bm->word);
			spin_lock(&ctx->lock);
			list_splice_tail_init(&ctx->rq_list, list);
			spin_unlock(&ctx->lock);

			bit++;
		} while (1);
	}
}
