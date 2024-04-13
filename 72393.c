static int check_asym_packing(struct lb_env *env, struct sd_lb_stats *sds)
{
	int busiest_cpu;

	if (!(env->sd->flags & SD_ASYM_PACKING))
		return 0;

	if (env->idle == CPU_NOT_IDLE)
		return 0;

	if (!sds->busiest)
		return 0;

	busiest_cpu = sds->busiest->asym_prefer_cpu;
	if (sched_asym_prefer(busiest_cpu, env->dst_cpu))
		return 0;

	env->imbalance = DIV_ROUND_CLOSEST(
		sds->busiest_stat.avg_load * sds->busiest_stat.group_capacity,
		SCHED_CAPACITY_SCALE);

	return 1;
}
