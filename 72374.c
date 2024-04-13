static unsigned long capacity_orig_of(int cpu)
{
	return cpu_rq(cpu)->cpu_capacity_orig;
}
