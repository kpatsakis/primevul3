static int __init blk_mq_init(void)
{
	blk_mq_cpu_init();

	hotcpu_notifier(blk_mq_queue_reinit_notify, 0);

	return 0;
}
