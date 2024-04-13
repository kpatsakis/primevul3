static void task_set_group_fair(struct task_struct *p)
{
	struct sched_entity *se = &p->se;

	set_task_rq(p, task_cpu(p));
	se->depth = se->parent ? se->parent->depth + 1 : 0;
}
