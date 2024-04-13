static int etm_event_init(struct perf_event *event)
{
	int ret = 0;

	if (event->attr.type != etm_pmu.type) {
		ret = -ENOENT;
		goto out;
	}

	ret = etm_addr_filters_alloc(event);
	if (ret)
		goto out;

	event->destroy = etm_event_destroy;
out:
	return ret;
}
