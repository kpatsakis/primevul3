static int __alloc_gigantic_page(unsigned long start_pfn,
				unsigned long nr_pages, gfp_t gfp_mask)
{
	unsigned long end_pfn = start_pfn + nr_pages;
	return alloc_contig_range(start_pfn, end_pfn, MIGRATE_MOVABLE,
				  gfp_mask);
}
