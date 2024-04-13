static inline bool cfs_bandwidth_used(void)
{
	return static_key_false(&__cfs_bandwidth_used);
}
