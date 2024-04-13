static void blk_set_congested(struct request_list *rl, int sync)
{
#ifdef CONFIG_CGROUP_WRITEBACK
	set_wb_congested(rl->blkg->wb_congested, sync);
#else
	/* see blk_clear_congested() */
	if (rl == &rl->q->root_rl)
		set_wb_congested(rl->q->backing_dev_info->wb.congested, sync);
#endif
}
