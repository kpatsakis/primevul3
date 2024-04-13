static void etm_addr_filters_sync(struct perf_event *event)
{
	struct perf_addr_filters_head *head = perf_event_addr_filters(event);
	unsigned long start, stop, *offs = event->addr_filters_offs;
	struct etm_filters *filters = event->hw.addr_filters;
	struct etm_filter *etm_filter;
	struct perf_addr_filter *filter;
	int i = 0;

	list_for_each_entry(filter, &head->list, entry) {
		start = filter->offset + offs[i];
		stop = start + filter->size;
		etm_filter = &filters->etm_filter[i];

		if (filter->range == 1) {
			etm_filter->start_addr = start;
			etm_filter->stop_addr = stop;
			etm_filter->type = ETM_ADDR_TYPE_RANGE;
		} else {
			if (filter->filter == 1) {
				etm_filter->start_addr = start;
				etm_filter->type = ETM_ADDR_TYPE_START;
			} else {
				etm_filter->stop_addr = stop;
				etm_filter->type = ETM_ADDR_TYPE_STOP;
			}
		}
		i++;
	}

	filters->nr_filters = i;
}
