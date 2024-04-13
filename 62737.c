filter_rsvd_memory (unsigned long start, unsigned long end, void *arg)
{
	unsigned long range_start, range_end, prev_start;
	void (*func)(unsigned long, unsigned long, int);
	int i;

#if IGNORE_PFN0
	if (start == PAGE_OFFSET) {
		printk(KERN_WARNING "warning: skipping physical page 0\n");
		start += PAGE_SIZE;
		if (start >= end) return 0;
	}
#endif
	/*
	 * lowest possible address(walker uses virtual)
	 */
	prev_start = PAGE_OFFSET;
	func = arg;

	for (i = 0; i < num_rsvd_regions; ++i) {
		range_start = max(start, prev_start);
		range_end   = min(end, rsvd_region[i].start);

		if (range_start < range_end)
			call_pernode_memory(__pa(range_start), range_end - range_start, func);

		/* nothing more available in this segment */
		if (range_end == end) return 0;

		prev_start = rsvd_region[i].end;
	}
	/* end of memory marker allows full processing inside loop body */
	return 0;
}
