int __weak arch_asym_cpu_priority(int cpu)
{
	return -cpu;
}
