group_has_capacity(struct lb_env *env, struct sg_lb_stats *sgs)
{
	if (sgs->sum_nr_running < sgs->group_weight)
		return true;

	if ((sgs->group_capacity * 100) >
			(sgs->group_util * env->sd->imbalance_pct))
		return true;

	return false;
}
