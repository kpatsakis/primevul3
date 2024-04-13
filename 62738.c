get_max_cacheline_size (void)
{
	unsigned long line_size, max = 1;
	u64 l, levels, unique_caches;
        pal_cache_config_info_t cci;
        s64 status;

        status = ia64_pal_cache_summary(&levels, &unique_caches);
        if (status != 0) {
                printk(KERN_ERR "%s: ia64_pal_cache_summary() failed (status=%ld)\n",
                       __func__, status);
                max = SMP_CACHE_BYTES;
		/* Safest setup for "flush_icache_range()" */
		ia64_i_cache_stride_shift = I_CACHE_STRIDE_SHIFT;
		goto out;
        }

	for (l = 0; l < levels; ++l) {
		status = ia64_pal_cache_config_info(l, /* cache_type (data_or_unified)= */ 2,
						    &cci);
		if (status != 0) {
			printk(KERN_ERR
			       "%s: ia64_pal_cache_config_info(l=%lu, 2) failed (status=%ld)\n",
			       __func__, l, status);
			max = SMP_CACHE_BYTES;
			/* The safest setup for "flush_icache_range()" */
			cci.pcci_stride = I_CACHE_STRIDE_SHIFT;
			cci.pcci_unified = 1;
		}
		line_size = 1 << cci.pcci_line_size;
		if (line_size > max)
			max = line_size;
		if (!cci.pcci_unified) {
			status = ia64_pal_cache_config_info(l,
						    /* cache_type (instruction)= */ 1,
						    &cci);
			if (status != 0) {
				printk(KERN_ERR
				"%s: ia64_pal_cache_config_info(l=%lu, 1) failed (status=%ld)\n",
					__func__, l, status);
				/* The safest setup for "flush_icache_range()" */
				cci.pcci_stride = I_CACHE_STRIDE_SHIFT;
			}
		}
		if (cci.pcci_stride < ia64_i_cache_stride_shift)
			ia64_i_cache_stride_shift = cci.pcci_stride;
	}
  out:
	if (max > ia64_max_cacheline_size)
		ia64_max_cacheline_size = max;
}
