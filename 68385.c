static void lo_complete_rq(struct request *rq)
{
	struct loop_cmd *cmd = blk_mq_rq_to_pdu(rq);

	if (unlikely(req_op(cmd->rq) == REQ_OP_READ && cmd->use_aio &&
		     cmd->ret >= 0 && cmd->ret < blk_rq_bytes(cmd->rq))) {
		struct bio *bio = cmd->rq->bio;

		bio_advance(bio, cmd->ret);
		zero_fill_bio(bio);
	}

	blk_mq_end_request(rq, cmd->ret < 0 ? BLK_STS_IOERR : BLK_STS_OK);
}
