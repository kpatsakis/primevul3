void blk_mq_disable_hotplug(void)
{
	mutex_lock(&all_q_mutex);
}
