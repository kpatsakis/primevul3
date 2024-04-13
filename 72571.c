static void task_dead_fair(struct task_struct *p)
{
	remove_entity_load_avg(&p->se);
}
