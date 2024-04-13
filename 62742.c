mark_bsp_online (void)
{
#ifdef CONFIG_SMP
	/* If we register an early console, allow CPU 0 to printk */
	cpu_set(smp_processor_id(), cpu_online_map);
#endif
}
