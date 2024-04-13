static int do_req_filebacked(struct loop_device *lo, struct request *rq)
{
	struct loop_cmd *cmd = blk_mq_rq_to_pdu(rq);
	loff_t pos = ((loff_t) blk_rq_pos(rq) << 9) + lo->lo_offset;

	/*
	 * lo_write_simple and lo_read_simple should have been covered
	 * by io submit style function like lo_rw_aio(), one blocker
	 * is that lo_read_simple() need to call flush_dcache_page after
	 * the page is written from kernel, and it isn't easy to handle
	 * this in io submit style function which submits all segments
	 * of the req at one time. And direct read IO doesn't need to
	 * run flush_dcache_page().
	 */
	switch (req_op(rq)) {
	case REQ_OP_FLUSH:
		return lo_req_flush(lo, rq);
	case REQ_OP_DISCARD:
	case REQ_OP_WRITE_ZEROES:
		return lo_discard(lo, rq, pos);
	case REQ_OP_WRITE:
		if (lo->transfer)
			return lo_write_transfer(lo, rq, pos);
		else if (cmd->use_aio)
			return lo_rw_aio(lo, cmd, pos, WRITE);
		else
			return lo_write_simple(lo, rq, pos);
	case REQ_OP_READ:
		if (lo->transfer)
			return lo_read_transfer(lo, rq, pos);
		else if (cmd->use_aio)
			return lo_rw_aio(lo, cmd, pos, READ);
		else
			return lo_read_simple(lo, rq, pos);
	default:
		WARN_ON_ONCE(1);
		return -EIO;
		break;
	}
}
