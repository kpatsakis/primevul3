void net_enable_timestamp(void)
{
#ifdef HAVE_JUMP_LABEL
	int wanted;

	while (1) {
		wanted = atomic_read(&netstamp_wanted);
		if (wanted <= 0)
			break;
		if (atomic_cmpxchg(&netstamp_wanted, wanted, wanted + 1) == wanted)
			return;
	}
	atomic_inc(&netstamp_needed_deferred);
	schedule_work(&netstamp_work);
#else
	static_key_slow_inc(&netstamp_needed);
#endif
}
