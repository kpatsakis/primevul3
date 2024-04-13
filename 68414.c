static void loop_handle_cmd(struct loop_cmd *cmd)
{
	const bool write = op_is_write(req_op(cmd->rq));
	struct loop_device *lo = cmd->rq->q->queuedata;
	int ret = 0;

	if (write && (lo->lo_flags & LO_FLAGS_READ_ONLY)) {
		ret = -EIO;
		goto failed;
	}

	ret = do_req_filebacked(lo, cmd->rq);
 failed:
	/* complete non-aio request */
	if (!cmd->use_aio || ret) {
		cmd->ret = ret ? -EIO : 0;
		blk_mq_complete_request(cmd->rq);
	}
}
