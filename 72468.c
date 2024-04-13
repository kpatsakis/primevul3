static inline unsigned long group_weight(struct task_struct *p, int nid,
					 int dist)
{
	unsigned long faults, total_faults;

	if (!p->numa_group)
		return 0;

	total_faults = p->numa_group->total_faults;

	if (!total_faults)
		return 0;

	faults = group_faults(p, nid);
	faults += score_nearby_nodes(p, nid, dist, false);

	return 1000 * faults / total_faults;
}
