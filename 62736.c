filter_memory(unsigned long start, unsigned long end, void *arg)
{
	void (*func)(unsigned long, unsigned long, int);

#if IGNORE_PFN0
	if (start == PAGE_OFFSET) {
		printk(KERN_WARNING "warning: skipping physical page 0\n");
		start += PAGE_SIZE;
		if (start >= end)
			return 0;
	}
#endif
	func = arg;
	if (start < end)
		call_pernode_memory(__pa(start), end - start, func);
	return 0;
}
