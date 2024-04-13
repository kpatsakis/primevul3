void cfs_bandwidth_usage_inc(void)
{
	static_key_slow_inc_cpuslocked(&__cfs_bandwidth_used);
}
