list_update_cgroup_event(struct perf_event *event,
			 struct perf_event_context *ctx, bool add)
{
	struct perf_cpu_context *cpuctx;
	struct list_head *cpuctx_entry;

	if (!is_cgroup_event(event))
		return;

	if (add && ctx->nr_cgroups++)
		return;
	else if (!add && --ctx->nr_cgroups)
		return;
	/*
	 * Because cgroup events are always per-cpu events,
	 * this will always be called from the right CPU.
	 */
	cpuctx = __get_cpu_context(ctx);
	cpuctx_entry = &cpuctx->cgrp_cpuctx_entry;
	/* cpuctx->cgrp is NULL unless a cgroup event is active in this CPU .*/
	if (add) {
		list_add(cpuctx_entry, this_cpu_ptr(&cgrp_cpuctx_list));
		if (perf_cgroup_from_task(current, ctx) == event->cgrp)
			cpuctx->cgrp = event->cgrp;
	} else {
		list_del(cpuctx_entry);
		cpuctx->cgrp = NULL;
	}
}
