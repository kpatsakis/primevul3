setup_per_cpu_areas (void)
{
	/* start_kernel() requires this... */
#ifdef CONFIG_ACPI_HOTPLUG_CPU
	prefill_possible_map();
#endif
}
