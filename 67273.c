static void etm_event_stop(struct perf_event *event, int mode)
{
	bool lost;
	int cpu = smp_processor_id();
	unsigned long size;
	struct coresight_device *sink, *csdev = per_cpu(csdev_src, cpu);
	struct perf_output_handle *handle = this_cpu_ptr(&ctx_handle);
	struct etm_event_data *event_data = perf_get_aux(handle);

	if (event->hw.state == PERF_HES_STOPPED)
		return;

	if (!csdev)
		return;

	sink = coresight_get_sink(event_data->path[cpu]);
	if (!sink)
		return;

	/* stop tracer */
	source_ops(csdev)->disable(csdev, event);

	/* tell the core */
	event->hw.state = PERF_HES_STOPPED;

	if (mode & PERF_EF_UPDATE) {
		if (WARN_ON_ONCE(handle->event != event))
			return;

		/* update trace information */
		if (!sink_ops(sink)->update_buffer)
			return;

		sink_ops(sink)->update_buffer(sink, handle,
					      event_data->snk_config);

		if (!sink_ops(sink)->reset_buffer)
			return;

		size = sink_ops(sink)->reset_buffer(sink, handle,
						    event_data->snk_config,
						    &lost);

		perf_aux_output_end(handle, size, lost);
	}

	/* Disabling the path make its elements available to other sessions */
	coresight_disable_path(event_data->path[cpu]);
}
