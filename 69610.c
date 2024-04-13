static int plug_ctx_cmp(void *priv, struct list_head *a, struct list_head *b)
{
	struct request *rqa = container_of(a, struct request, queuelist);
	struct request *rqb = container_of(b, struct request, queuelist);

	return !(rqa->mq_ctx < rqb->mq_ctx ||
		 (rqa->mq_ctx == rqb->mq_ctx &&
		  blk_rq_pos(rqa) < blk_rq_pos(rqb)));
}
