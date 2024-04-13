static struct page *alloc_gigantic_page(int nid, struct hstate *h)
{
	unsigned int order = huge_page_order(h);
	unsigned long nr_pages = 1 << order;
	unsigned long ret, pfn, flags;
	struct zonelist *zonelist;
	struct zone *zone;
	struct zoneref *z;
	gfp_t gfp_mask;

	gfp_mask = htlb_alloc_mask(h) | __GFP_THISNODE;
	zonelist = node_zonelist(nid, gfp_mask);
	for_each_zone_zonelist_nodemask(zone, z, zonelist, gfp_zone(gfp_mask), NULL) {
		spin_lock_irqsave(&zone->lock, flags);

		pfn = ALIGN(zone->zone_start_pfn, nr_pages);
		while (zone_spans_last_pfn(zone, pfn, nr_pages)) {
			if (pfn_range_valid_gigantic(zone, pfn, nr_pages)) {
				/*
				 * We release the zone lock here because
				 * alloc_contig_range() will also lock the zone
				 * at some point. If there's an allocation
				 * spinning on this lock, it may win the race
				 * and cause alloc_contig_range() to fail...
				 */
				spin_unlock_irqrestore(&zone->lock, flags);
				ret = __alloc_gigantic_page(pfn, nr_pages, gfp_mask);
				if (!ret)
					return pfn_to_page(pfn);
				spin_lock_irqsave(&zone->lock, flags);
			}
			pfn += nr_pages;
		}

		spin_unlock_irqrestore(&zone->lock, flags);
	}

	return NULL;
}
