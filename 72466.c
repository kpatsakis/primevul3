group_smaller_max_cpu_capacity(struct sched_group *sg, struct sched_group *ref)
{
	return sg->sgc->max_capacity * capacity_margin <
						ref->sgc->max_capacity * 1024;
}
