static unsigned int task_scan_start(struct task_struct *p)
{
	unsigned long smin = task_scan_min(p);
	unsigned long period = smin;

	/* Scale the maximum scan period with the amount of shared memory. */
	if (p->numa_group) {
		struct numa_group *ng = p->numa_group;
		unsigned long shared = group_faults_shared(ng);
		unsigned long private = group_faults_priv(ng);

		period *= atomic_read(&ng->refcount);
		period *= shared + 1;
		period /= private + shared + 1;
	}

	return max(smin, period);
}
