static unsigned int blk_flush_cur_seq(struct request *rq)
{
	return 1 << ffz(rq->flush.seq);
}
