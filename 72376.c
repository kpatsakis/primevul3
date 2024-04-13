void cfs_bandwidth_usage_dec(void)
{
	static_key_slow_dec_cpuslocked(&__cfs_bandwidth_used);
}
