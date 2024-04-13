unsigned int blk_plug_queued_count(struct request_queue *q)
{
	struct blk_plug *plug;
	struct request *rq;
	struct list_head *plug_list;
	unsigned int ret = 0;

	plug = current->plug;
	if (!plug)
		goto out;

	if (q->mq_ops)
		plug_list = &plug->mq_list;
	else
		plug_list = &plug->list;

	list_for_each_entry(rq, plug_list, queuelist) {
		if (rq->q == q)
			ret++;
	}
out:
	return ret;
}
