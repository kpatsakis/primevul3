void blk_mq_stop_hw_queue(struct blk_mq_hw_ctx *hctx)
{
	cancel_delayed_work(&hctx->run_work);
	cancel_delayed_work(&hctx->delay_work);
	set_bit(BLK_MQ_S_STOPPED, &hctx->state);
}
