bool blk_poll(struct request_queue *q, blk_qc_t cookie)
{
	if (!q->poll_fn || !blk_qc_t_valid(cookie))
		return false;

	if (current->plug)
		blk_flush_plug_list(current->plug, false);
	return q->poll_fn(q, cookie);
}
