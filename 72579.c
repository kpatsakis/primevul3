static void task_numa_assign(struct task_numa_env *env,
			     struct task_struct *p, long imp)
{
	struct rq *rq = cpu_rq(env->dst_cpu);

	/* Bail out if run-queue part of active NUMA balance. */
	if (xchg(&rq->numa_migrate_on, 1))
		return;

	/*
	 * Clear previous best_cpu/rq numa-migrate flag, since task now
	 * found a better CPU to move/swap.
	 */
	if (env->best_cpu != -1) {
		rq = cpu_rq(env->best_cpu);
		WRITE_ONCE(rq->numa_migrate_on, 0);
	}

	if (env->best_task)
		put_task_struct(env->best_task);
	if (p)
		get_task_struct(p);

	env->best_task = p;
	env->best_imp = imp;
	env->best_cpu = env->dst_cpu;
}
