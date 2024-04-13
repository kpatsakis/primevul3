static void blk_sq_make_request(struct request_queue *q, struct bio *bio)
{
	const int is_sync = rw_is_sync(bio->bi_rw);
	const int is_flush_fua = bio->bi_rw & (REQ_FLUSH | REQ_FUA);
	struct blk_plug *plug;
	unsigned int request_count = 0;
	struct blk_map_ctx data;
	struct request *rq;

	blk_queue_bounce(q, &bio);

	if (bio_integrity_enabled(bio) && bio_integrity_prep(bio)) {
		bio_io_error(bio);
		return;
	}

	blk_queue_split(q, &bio, q->bio_split);

	if (!is_flush_fua && !blk_queue_nomerges(q) &&
	    blk_attempt_plug_merge(q, bio, &request_count, NULL))
		return;

	rq = blk_mq_map_request(q, bio, &data);
	if (unlikely(!rq))
		return;

	if (unlikely(is_flush_fua)) {
		blk_mq_bio_to_request(rq, bio);
		blk_insert_flush(rq);
		goto run_queue;
	}

	/*
	 * A task plug currently exists. Since this is completely lockless,
	 * utilize that to temporarily store requests until the task is
	 * either done or scheduled away.
	 */
	plug = current->plug;
	if (plug) {
		blk_mq_bio_to_request(rq, bio);
		if (list_empty(&plug->mq_list))
			trace_block_plug(q);
		else if (request_count >= BLK_MAX_REQUEST_COUNT) {
			blk_flush_plug_list(plug, false);
			trace_block_plug(q);
		}
		list_add_tail(&rq->queuelist, &plug->mq_list);
		blk_mq_put_ctx(data.ctx);
		return;
	}

	if (!blk_mq_merge_queue_io(data.hctx, data.ctx, rq, bio)) {
		/*
		 * For a SYNC request, send it to the hardware immediately. For
		 * an ASYNC request, just ensure that we run it later on. The
		 * latter allows for merging opportunities and more efficient
		 * dispatching.
		 */
run_queue:
		blk_mq_run_hw_queue(data.hctx, !is_sync || is_flush_fua);
	}

	blk_mq_put_ctx(data.ctx);
}
