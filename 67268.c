static int etm_event_add(struct perf_event *event, int mode)
{
	int ret = 0;
	struct hw_perf_event *hwc = &event->hw;

	if (mode & PERF_EF_START) {
		etm_event_start(event, 0);
		if (hwc->state & PERF_HES_STOPPED)
			ret = -EINVAL;
	} else {
		hwc->state = PERF_HES_STOPPED;
	}

	return ret;
}
