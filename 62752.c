show_cpuinfo (struct seq_file *m, void *v)
{
#ifdef CONFIG_SMP
#	define lpj	c->loops_per_jiffy
#	define cpunum	c->cpu
#else
#	define lpj	loops_per_jiffy
#	define cpunum	0
#endif
	static struct {
		unsigned long mask;
		const char *feature_name;
	} feature_bits[] = {
		{ 1UL << 0, "branchlong" },
		{ 1UL << 1, "spontaneous deferral"},
		{ 1UL << 2, "16-byte atomic ops" }
	};
	char features[128], *cp, *sep;
	struct cpuinfo_ia64 *c = v;
	unsigned long mask;
	unsigned long proc_freq;
	int i, size;

	mask = c->features;

	/* build the feature string: */
	memcpy(features, "standard", 9);
	cp = features;
	size = sizeof(features);
	sep = "";
	for (i = 0; i < ARRAY_SIZE(feature_bits) && size > 1; ++i) {
		if (mask & feature_bits[i].mask) {
			cp += snprintf(cp, size, "%s%s", sep,
				       feature_bits[i].feature_name),
			sep = ", ";
			mask &= ~feature_bits[i].mask;
			size = sizeof(features) - (cp - features);
		}
	}
	if (mask && size > 1) {
		/* print unknown features as a hex value */
		snprintf(cp, size, "%s0x%lx", sep, mask);
	}

	proc_freq = cpufreq_quick_get(cpunum);
	if (!proc_freq)
		proc_freq = c->proc_freq / 1000;

	seq_printf(m,
		   "processor  : %d\n"
		   "vendor     : %s\n"
		   "arch       : IA-64\n"
		   "family     : %u\n"
		   "model      : %u\n"
		   "model name : %s\n"
		   "revision   : %u\n"
		   "archrev    : %u\n"
		   "features   : %s\n"
		   "cpu number : %lu\n"
		   "cpu regs   : %u\n"
		   "cpu MHz    : %lu.%03lu\n"
		   "itc MHz    : %lu.%06lu\n"
		   "BogoMIPS   : %lu.%02lu\n",
		   cpunum, c->vendor, c->family, c->model,
		   c->model_name, c->revision, c->archrev,
		   features, c->ppn, c->number,
		   proc_freq / 1000, proc_freq % 1000,
		   c->itc_freq / 1000000, c->itc_freq % 1000000,
		   lpj*HZ/500000, (lpj*HZ/5000) % 100);
#ifdef CONFIG_SMP
	seq_printf(m, "siblings   : %u\n", cpus_weight(cpu_core_map[cpunum]));
	if (c->socket_id != -1)
		seq_printf(m, "physical id: %u\n", c->socket_id);
	if (c->threads_per_core > 1 || c->cores_per_socket > 1)
		seq_printf(m,
			   "core id    : %u\n"
			   "thread id  : %u\n",
			   c->core_id, c->thread_id);
#endif
	seq_printf(m,"\n");

	return 0;
}
