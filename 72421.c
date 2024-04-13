static void destroy_cfs_bandwidth(struct cfs_bandwidth *cfs_b)
{
	/* init_cfs_bandwidth() was not called */
	if (!cfs_b->throttled_cfs_rq.next)
		return;

	hrtimer_cancel(&cfs_b->period_timer);
	hrtimer_cancel(&cfs_b->slack_timer);
}
