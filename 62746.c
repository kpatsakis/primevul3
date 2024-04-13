reserve_memory (void)
{
	int n = 0;
	unsigned long total_memory;

	/*
	 * none of the entries in this table overlap
	 */
	rsvd_region[n].start = (unsigned long) ia64_boot_param;
	rsvd_region[n].end   = rsvd_region[n].start + sizeof(*ia64_boot_param);
	n++;

	rsvd_region[n].start = (unsigned long) __va(ia64_boot_param->efi_memmap);
	rsvd_region[n].end   = rsvd_region[n].start + ia64_boot_param->efi_memmap_size;
	n++;

	rsvd_region[n].start = (unsigned long) __va(ia64_boot_param->command_line);
	rsvd_region[n].end   = (rsvd_region[n].start
				+ strlen(__va(ia64_boot_param->command_line)) + 1);
	n++;

	rsvd_region[n].start = (unsigned long) ia64_imva((void *)KERNEL_START);
	rsvd_region[n].end   = (unsigned long) ia64_imva(_end);
	n++;

#ifdef CONFIG_BLK_DEV_INITRD
	if (ia64_boot_param->initrd_start) {
		rsvd_region[n].start = (unsigned long)__va(ia64_boot_param->initrd_start);
		rsvd_region[n].end   = rsvd_region[n].start + ia64_boot_param->initrd_size;
		n++;
	}
#endif

#ifdef CONFIG_PROC_VMCORE
	if (reserve_elfcorehdr(&rsvd_region[n].start,
			       &rsvd_region[n].end) == 0)
		n++;
#endif

	total_memory = efi_memmap_init(&rsvd_region[n].start, &rsvd_region[n].end);
	n++;

	setup_crashkernel(total_memory, &n);

	/* end of memory marker */
	rsvd_region[n].start = ~0UL;
	rsvd_region[n].end   = ~0UL;
	n++;

	num_rsvd_regions = n;
	BUG_ON(IA64_MAX_RSVD_REGIONS + 1 < n);

	sort_regions(rsvd_region, num_rsvd_regions);
}
