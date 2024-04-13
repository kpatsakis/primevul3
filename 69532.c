__blk_mq_alloc_request(struct blk_mq_alloc_data *data, int rw)
{
	struct request *rq;
	unsigned int tag;

	tag = blk_mq_get_tag(data);
	if (tag != BLK_MQ_TAG_FAIL) {
		rq = data->hctx->tags->rqs[tag];

		if (blk_mq_tag_busy(data->hctx)) {
			rq->cmd_flags = REQ_MQ_INFLIGHT;
			atomic_inc(&data->hctx->nr_active);
		}

		rq->tag = tag;
		blk_mq_rq_ctx_init(data->q, data->ctx, rq, rw);
		return rq;
	}

	return NULL;
}
