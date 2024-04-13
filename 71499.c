static int __meminit init_reserve_notifier(void)
{
	if (register_hotmemory_notifier(&reserve_mem_nb))
		pr_err("Failed registering memory add/remove notifier for admin reserve\n");

	return 0;
}
