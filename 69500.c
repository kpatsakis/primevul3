static void blk_flush_restore_request(struct request *rq)
{
	/*
	 * After flush data completion, @rq->bio is %NULL but we need to
	 * complete the bio again.  @rq->biotail is guaranteed to equal the
	 * original @rq->bio.  Restore it.
	 */
	rq->bio = rq->biotail;

	/* make @rq a normal request */
	rq->cmd_flags &= ~REQ_FLUSH_SEQ;
	rq->end_io = rq->flush.saved_end_io;
}
