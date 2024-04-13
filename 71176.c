void debug_timestamp(char *msg)
{
	struct timespec64 t;

	ktime_get_ts64(&t);
	pr_debug("**%s: %lld.%9.9ld\n", msg, (long long) t.tv_sec, t.tv_nsec);
}
