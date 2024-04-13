static int madvise_inject_error(int behavior,
		unsigned long start, unsigned long end)
{
	struct page *page;
	struct zone *zone;
	unsigned int order;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;


	for (; start < end; start += PAGE_SIZE << order) {
		int ret;

		ret = get_user_pages_fast(start, 1, 0, &page);
		if (ret != 1)
			return ret;

		/*
		 * When soft offlining hugepages, after migrating the page
		 * we dissolve it, therefore in the second loop "page" will
		 * no longer be a compound page, and order will be 0.
		 */
		order = compound_order(compound_head(page));

		if (PageHWPoison(page)) {
			put_page(page);
			continue;
		}

		if (behavior == MADV_SOFT_OFFLINE) {
			pr_info("Soft offlining pfn %#lx at process virtual address %#lx\n",
						page_to_pfn(page), start);

			ret = soft_offline_page(page, MF_COUNT_INCREASED);
			if (ret)
				return ret;
			continue;
		}
		pr_info("Injecting memory failure for pfn %#lx at process virtual address %#lx\n",
						page_to_pfn(page), start);

		ret = memory_failure(page_to_pfn(page), 0, MF_COUNT_INCREASED);
		if (ret)
			return ret;
	}

	/* Ensure that all poisoned pages are removed from per-cpu lists */
	for_each_populated_zone(zone)
		drain_all_pages(zone);

	return 0;
}
