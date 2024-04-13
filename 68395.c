static void lo_rw_aio_do_completion(struct loop_cmd *cmd)
{
	if (!atomic_dec_and_test(&cmd->ref))
		return;
	kfree(cmd->bvec);
	cmd->bvec = NULL;
	blk_mq_complete_request(cmd->rq);
}
