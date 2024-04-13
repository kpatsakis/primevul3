static inline int get_sd_load_idx(struct sched_domain *sd,
					enum cpu_idle_type idle)
{
	int load_idx;

	switch (idle) {
	case CPU_NOT_IDLE:
		load_idx = sd->busy_idx;
		break;

	case CPU_NEWLY_IDLE:
		load_idx = sd->newidle_idx;
		break;
	default:
		load_idx = sd->idle_idx;
		break;
	}

	return load_idx;
}
