static inline void update_misfit_status(struct task_struct *p, struct rq *rq)
{
	if (!static_branch_unlikely(&sched_asym_cpucapacity))
		return;

	if (!p) {
		rq->misfit_task_load = 0;
		return;
	}

	if (task_fits_capacity(p, capacity_of(cpu_of(rq)))) {
		rq->misfit_task_load = 0;
		return;
	}

	rq->misfit_task_load = task_h_load(p);
}
