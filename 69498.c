static unsigned int blk_flush_policy(unsigned int fflags, struct request *rq)
{
	unsigned int policy = 0;

	if (blk_rq_sectors(rq))
		policy |= REQ_FSEQ_DATA;

	if (fflags & REQ_FLUSH) {
		if (rq->cmd_flags & REQ_FLUSH)
			policy |= REQ_FSEQ_PREFLUSH;
		if (!(fflags & REQ_FUA) && (rq->cmd_flags & REQ_FUA))
			policy |= REQ_FSEQ_POSTFLUSH;
	}
	return policy;
}
