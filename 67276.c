static void free_event_data(struct work_struct *work)
{
	int cpu;
	cpumask_t *mask;
	struct etm_event_data *event_data;
	struct coresight_device *sink;

	event_data = container_of(work, struct etm_event_data, work);
	mask = &event_data->mask;
	/*
	 * First deal with the sink configuration.  See comment in
	 * etm_setup_aux() about why we take the first available path.
	 */
	if (event_data->snk_config) {
		cpu = cpumask_first(mask);
		sink = coresight_get_sink(event_data->path[cpu]);
		if (sink_ops(sink)->free_buffer)
			sink_ops(sink)->free_buffer(event_data->snk_config);
	}

	for_each_cpu(cpu, mask) {
		if (!(IS_ERR_OR_NULL(event_data->path[cpu])))
			coresight_release_path(event_data->path[cpu]);
	}

	kfree(event_data->path);
	kfree(event_data);
}
