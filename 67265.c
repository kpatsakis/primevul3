static int etm_addr_filters_alloc(struct perf_event *event)
{
	struct etm_filters *filters;
	int node = event->cpu == -1 ? -1 : cpu_to_node(event->cpu);

	filters = kzalloc_node(sizeof(struct etm_filters), GFP_KERNEL, node);
	if (!filters)
		return -ENOMEM;

	if (event->parent)
		memcpy(filters, event->parent->hw.addr_filters,
		       sizeof(*filters));

	event->hw.addr_filters = filters;

	return 0;
}
