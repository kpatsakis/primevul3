void update_group_capacity(struct sched_domain *sd, int cpu)
{
	struct sched_domain *child = sd->child;
	struct sched_group *group, *sdg = sd->groups;
	unsigned long capacity, min_capacity, max_capacity;
	unsigned long interval;

	interval = msecs_to_jiffies(sd->balance_interval);
	interval = clamp(interval, 1UL, max_load_balance_interval);
	sdg->sgc->next_update = jiffies + interval;

	if (!child) {
		update_cpu_capacity(sd, cpu);
		return;
	}

	capacity = 0;
	min_capacity = ULONG_MAX;
	max_capacity = 0;

	if (child->flags & SD_OVERLAP) {
		/*
		 * SD_OVERLAP domains cannot assume that child groups
		 * span the current group.
		 */

		for_each_cpu(cpu, sched_group_span(sdg)) {
			struct sched_group_capacity *sgc;
			struct rq *rq = cpu_rq(cpu);

			/*
			 * build_sched_domains() -> init_sched_groups_capacity()
			 * gets here before we've attached the domains to the
			 * runqueues.
			 *
			 * Use capacity_of(), which is set irrespective of domains
			 * in update_cpu_capacity().
			 *
			 * This avoids capacity from being 0 and
			 * causing divide-by-zero issues on boot.
			 */
			if (unlikely(!rq->sd)) {
				capacity += capacity_of(cpu);
			} else {
				sgc = rq->sd->groups->sgc;
				capacity += sgc->capacity;
			}

			min_capacity = min(capacity, min_capacity);
			max_capacity = max(capacity, max_capacity);
		}
	} else  {
		/*
		 * !SD_OVERLAP domains can assume that child groups
		 * span the current group.
		 */

		group = child->groups;
		do {
			struct sched_group_capacity *sgc = group->sgc;

			capacity += sgc->capacity;
			min_capacity = min(sgc->min_capacity, min_capacity);
			max_capacity = max(sgc->max_capacity, max_capacity);
			group = group->next;
		} while (group != child->groups);
	}

	sdg->sgc->capacity = capacity;
	sdg->sgc->min_capacity = min_capacity;
	sdg->sgc->max_capacity = max_capacity;
}
