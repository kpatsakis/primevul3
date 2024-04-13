static void __blk_mq_requeue_request(struct request *rq)
{
	struct request_queue *q = rq->q;

	trace_block_rq_requeue(q, rq);

	if (test_and_clear_bit(REQ_ATOM_STARTED, &rq->atomic_flags)) {
		if (q->dma_drain_size && blk_rq_bytes(rq))
			rq->nr_phys_segments--;
	}
}
