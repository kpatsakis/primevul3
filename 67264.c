static void *alloc_event_data(int cpu)
{
	int size;
	cpumask_t *mask;
	struct etm_event_data *event_data;

	/* First get memory for the session's data */
	event_data = kzalloc(sizeof(struct etm_event_data), GFP_KERNEL);
	if (!event_data)
		return NULL;

	/* Make sure nothing disappears under us */
	get_online_cpus();
	size = num_online_cpus();

	mask = &event_data->mask;
	if (cpu != -1)
		cpumask_set_cpu(cpu, mask);
	else
		cpumask_copy(mask, cpu_online_mask);
	put_online_cpus();

	/*
	 * Each CPU has a single path between source and destination.  As such
	 * allocate an array using CPU numbers as indexes.  That way a path
	 * for any CPU can easily be accessed at any given time.  We proceed
	 * the same way for sessions involving a single CPU.  The cost of
	 * unused memory when dealing with single CPU trace scenarios is small
	 * compared to the cost of searching through an optimized array.
	 */
	event_data->path = kcalloc(size,
				   sizeof(struct list_head *), GFP_KERNEL);
	if (!event_data->path) {
		kfree(event_data);
		return NULL;
	}

	return event_data;
}
