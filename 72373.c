static unsigned long capacity_of(int cpu)
{
	return cpu_rq(cpu)->cpu_capacity;
}
