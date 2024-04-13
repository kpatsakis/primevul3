static void get_old_counters(const struct xt_table_info *t,
			     struct xt_counters counters[])
{
	struct ip6t_entry *iter;
	unsigned int cpu, i;

	for_each_possible_cpu(cpu) {
		i = 0;
		xt_entry_foreach(iter, t->entries, t->size) {
			const struct xt_counters *tmp;

			tmp = xt_get_per_cpu_counter(&iter->counters, cpu);
			ADD_COUNTER(counters[i], tmp->bcnt, tmp->pcnt);
			++i;
		}
		cond_resched();
	}
}
