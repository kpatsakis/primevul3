c_start (struct seq_file *m, loff_t *pos)
{
#ifdef CONFIG_SMP
	while (*pos < NR_CPUS && !cpu_isset(*pos, cpu_online_map))
		++*pos;
#endif
	return *pos < NR_CPUS ? cpu_data(*pos) : NULL;
}
