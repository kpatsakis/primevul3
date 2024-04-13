static int loop_set_block_size(struct loop_device *lo, unsigned long arg)
{
	if (lo->lo_state != Lo_bound)
		return -ENXIO;

	if (arg < 512 || arg > PAGE_SIZE || !is_power_of_2(arg))
		return -EINVAL;

	blk_mq_freeze_queue(lo->lo_queue);

	blk_queue_logical_block_size(lo->lo_queue, arg);
	blk_queue_physical_block_size(lo->lo_queue, arg);
	blk_queue_io_min(lo->lo_queue, arg);
	loop_update_dio(lo);

	blk_mq_unfreeze_queue(lo->lo_queue);

	return 0;
}
