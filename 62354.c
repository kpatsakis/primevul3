static int einj_timedout(u64 *t)
{
	if ((s64)*t < SPIN_UNIT) {
		pr_warning(FW_WARN EINJ_PFX
			   "Firmware does not respond in time\n");
		return 1;
	}
	*t -= SPIN_UNIT;
	ndelay(SPIN_UNIT);
	touch_nmi_watchdog();
	return 0;
}
