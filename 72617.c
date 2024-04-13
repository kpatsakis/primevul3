static void update_cpu_capacity(struct sched_domain *sd, int cpu)
{
	unsigned long capacity = scale_rt_capacity(sd, cpu);
	struct sched_group *sdg = sd->groups;

	cpu_rq(cpu)->cpu_capacity_orig = arch_scale_cpu_capacity(sd, cpu);

	if (!capacity)
		capacity = 1;

	cpu_rq(cpu)->cpu_capacity = capacity;
	sdg->sgc->capacity = capacity;
	sdg->sgc->min_capacity = capacity;
	sdg->sgc->max_capacity = capacity;
}
