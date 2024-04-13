group_smaller_min_cpu_capacity(struct sched_group *sg, struct sched_group *ref)
{
	return sg->sgc->min_capacity * capacity_margin <
						ref->sgc->min_capacity * 1024;
}
