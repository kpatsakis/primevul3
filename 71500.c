static int init_user_reserve(void)
{
	unsigned long free_kbytes;

	free_kbytes = global_zone_page_state(NR_FREE_PAGES) << (PAGE_SHIFT - 10);

	sysctl_user_reserve_kbytes = min(free_kbytes / 32, 1UL << 17);
	return 0;
}
