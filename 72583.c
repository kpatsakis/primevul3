pid_t task_numa_group_id(struct task_struct *p)
{
	return p->numa_group ? p->numa_group->gid : 0;
}
