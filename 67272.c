static void etm_event_start(struct perf_event *event, int flags)
{
	int cpu = smp_processor_id();
	struct etm_event_data *event_data;
	struct perf_output_handle *handle = this_cpu_ptr(&ctx_handle);
	struct coresight_device *sink, *csdev = per_cpu(csdev_src, cpu);

	if (!csdev)
		goto fail;

	/*
	 * Deal with the ring buffer API and get a handle on the
	 * session's information.
	 */
	event_data = perf_aux_output_begin(handle, event);
	if (!event_data)
		goto fail;

	/* We need a sink, no need to continue without one */
	sink = coresight_get_sink(event_data->path[cpu]);
	if (WARN_ON_ONCE(!sink || !sink_ops(sink)->set_buffer))
		goto fail_end_stop;

	/* Configure the sink */
	if (sink_ops(sink)->set_buffer(sink, handle,
				       event_data->snk_config))
		goto fail_end_stop;

	/* Nothing will happen without a path */
	if (coresight_enable_path(event_data->path[cpu], CS_MODE_PERF))
		goto fail_end_stop;

	/* Tell the perf core the event is alive */
	event->hw.state = 0;

	/* Finally enable the tracer */
	if (source_ops(csdev)->enable(csdev, event, CS_MODE_PERF))
		goto fail_end_stop;

out:
	return;

fail_end_stop:
	perf_aux_output_end(handle, 0, true);
fail:
	event->hw.state = PERF_HES_STOPPED;
	goto out;
}
