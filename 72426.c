static struct task_struct *detach_one_task(struct lb_env *env)
{
	struct task_struct *p;

	lockdep_assert_held(&env->src_rq->lock);

	list_for_each_entry_reverse(p,
			&env->src_rq->cfs_tasks, se.group_node) {
		if (!can_migrate_task(p, env))
			continue;

		detach_task(p, env);

		/*
		 * Right now, this is only the second place where
		 * lb_gained[env->idle] is updated (other is detach_tasks)
		 * so we can safely collect stats here rather than
		 * inside detach_tasks().
		 */
		schedstat_inc(env->sd->lb_gained[env->idle]);
		return p;
	}
	return NULL;
}
