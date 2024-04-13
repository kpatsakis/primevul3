static void motor_off_callback(struct timer_list *t)
{
	unsigned long nr = t - motor_off_timer;
	unsigned char mask = ~(0x10 << UNIT(nr));

	if (WARN_ON_ONCE(nr >= N_DRIVE))
		return;

	set_dor(FDC(nr), mask, 0);
}
