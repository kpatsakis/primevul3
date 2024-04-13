static void __init setup_crashkernel(unsigned long total, int *n)
{
	unsigned long long base = 0, size = 0;
	int ret;

	ret = parse_crashkernel(boot_command_line, total,
			&size, &base);
	if (ret == 0 && size > 0) {
		if (!base) {
			sort_regions(rsvd_region, *n);
			base = kdump_find_rsvd_region(size,
					rsvd_region, *n);
		}

		if (!check_crashkernel_memory(base, size)) {
			pr_warning("crashkernel: There would be kdump memory "
				"at %ld GB but this is unusable because it "
				"must\nbe below 4 GB. Change the memory "
				"configuration of the machine.\n",
				(unsigned long)(base >> 30));
			return;
		}

		if (base != ~0UL) {
			printk(KERN_INFO "Reserving %ldMB of memory at %ldMB "
					"for crashkernel (System RAM: %ldMB)\n",
					(unsigned long)(size >> 20),
					(unsigned long)(base >> 20),
					(unsigned long)(total >> 20));
			rsvd_region[*n].start =
				(unsigned long)__va(base);
			rsvd_region[*n].end =
				(unsigned long)__va(base + size);
			(*n)++;
			crashk_res.start = base;
			crashk_res.end = base + size - 1;
		}
	}
	efi_memmap_res.start = ia64_boot_param->efi_memmap;
	efi_memmap_res.end = efi_memmap_res.start +
		ia64_boot_param->efi_memmap_size;
	boot_param_res.start = __pa(ia64_boot_param);
	boot_param_res.end = boot_param_res.start +
		sizeof(*ia64_boot_param);
}
