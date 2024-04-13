static int reserve_mem_notifier(struct notifier_block *nb,
			     unsigned long action, void *data)
{
	unsigned long tmp, free_kbytes;

	switch (action) {
	case MEM_ONLINE:
		/* Default max is 128MB. Leave alone if modified by operator. */
		tmp = sysctl_user_reserve_kbytes;
		if (0 < tmp && tmp < (1UL << 17))
			init_user_reserve();

		/* Default max is 8MB.  Leave alone if modified by operator. */
		tmp = sysctl_admin_reserve_kbytes;
		if (0 < tmp && tmp < (1UL << 13))
			init_admin_reserve();

		break;
	case MEM_OFFLINE:
		free_kbytes = global_zone_page_state(NR_FREE_PAGES) << (PAGE_SHIFT - 10);

		if (sysctl_user_reserve_kbytes > free_kbytes) {
			init_user_reserve();
			pr_info("vm.user_reserve_kbytes reset to %lu\n",
				sysctl_user_reserve_kbytes);
		}

		if (sysctl_admin_reserve_kbytes > free_kbytes) {
			init_admin_reserve();
			pr_info("vm.admin_reserve_kbytes reset to %lu\n",
				sysctl_admin_reserve_kbytes);
		}
		break;
	default:
		break;
	}
	return NOTIFY_OK;
}
