int die(const char *str, struct pt_regs *regs, long err)
{
	static int die_counter;

	if (debugger(regs))
		return 1;

	console_verbose();
	spin_lock_irq(&die_lock);
	bust_spinlocks(1);
#ifdef CONFIG_PMAC_BACKLIGHT
	mutex_lock(&pmac_backlight_mutex);
	if (machine_is(powermac) && pmac_backlight) {
		struct backlight_properties *props;

		down(&pmac_backlight->sem);
		props = pmac_backlight->props;
		props->brightness = props->max_brightness;
		props->power = FB_BLANK_UNBLANK;
		props->update_status(pmac_backlight);
		up(&pmac_backlight->sem);
	}
	mutex_unlock(&pmac_backlight_mutex);
#endif
	printk("Oops: %s, sig: %ld [#%d]\n", str, err, ++die_counter);
#ifdef CONFIG_PREEMPT
	printk("PREEMPT ");
#endif
#ifdef CONFIG_SMP
	printk("SMP NR_CPUS=%d ", NR_CPUS);
#endif
#ifdef CONFIG_DEBUG_PAGEALLOC
	printk("DEBUG_PAGEALLOC ");
#endif
#ifdef CONFIG_NUMA
	printk("NUMA ");
#endif
	printk("%s\n", ppc_md.name ? "" : ppc_md.name);

	print_modules();
	show_regs(regs);
	bust_spinlocks(0);
	spin_unlock_irq(&die_lock);

	if (kexec_should_crash(current) ||
		kexec_sr_activated(smp_processor_id()))
		crash_kexec(regs);
	crash_kexec_secondary(regs);

	if (in_interrupt())
		panic("Fatal exception in interrupt");

	if (panic_on_oops)
		panic("Fatal exception");

	do_exit(err);

	return 0;
}
