void blk_free_flush_queue(struct blk_flush_queue *fq)
{
	/* bio based request queue hasn't flush queue */
	if (!fq)
		return;

	kfree(fq->flush_rq);
	kfree(fq);
}
