static inline int ipmi_thread_busy_wait(enum si_sm_result smi_result,
					const struct smi_info *smi_info,
					struct timespec64 *busy_until)
{
	unsigned int max_busy_us = 0;

	if (smi_info->si_num < num_max_busy_us)
		max_busy_us = kipmid_max_busy_us[smi_info->si_num];
	if (max_busy_us == 0 || smi_result != SI_SM_CALL_WITH_DELAY)
		ipmi_si_set_not_busy(busy_until);
	else if (!ipmi_si_is_busy(busy_until)) {
		ktime_get_ts64(busy_until);
		timespec64_add_ns(busy_until, max_busy_us*NSEC_PER_USEC);
	} else {
		struct timespec64 now;

		ktime_get_ts64(&now);
		if (unlikely(timespec64_compare(&now, busy_until) > 0)) {
			ipmi_si_set_not_busy(busy_until);
			return 0;
		}
	}
	return 1;
}
