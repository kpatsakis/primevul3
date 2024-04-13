static int loop_init_request(struct blk_mq_tag_set *set, struct request *rq,
		unsigned int hctx_idx, unsigned int numa_node)
{
	struct loop_cmd *cmd = blk_mq_rq_to_pdu(rq);

	cmd->rq = rq;
	kthread_init_work(&cmd->work, loop_queue_work);

	return 0;
}
