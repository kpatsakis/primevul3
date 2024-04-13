static void etm_event_destroy(struct perf_event *event)
{
	kfree(event->hw.addr_filters);
	event->hw.addr_filters = NULL;
}
