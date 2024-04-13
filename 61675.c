_waiter_create(uint32_t jobid)
{
	struct waiter *wp = xmalloc(sizeof(struct waiter));

	wp->jobid = jobid;
	wp->thd   = pthread_self();

	return wp;
}
