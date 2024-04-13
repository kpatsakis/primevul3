static void etm_event_del(struct perf_event *event, int mode)
{
	etm_event_stop(event, PERF_EF_UPDATE);
}
