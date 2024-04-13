static inline int ipmi_si_is_busy(struct timespec64 *ts)
{
	return ts->tv_nsec != -1;
}
