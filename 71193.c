static inline void ipmi_si_set_not_busy(struct timespec64 *ts)
{
	ts->tv_nsec = -1;
}
