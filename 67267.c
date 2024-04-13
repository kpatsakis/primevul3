static int etm_addr_filters_validate(struct list_head *filters)
{
	bool range = false, address = false;
	int index = 0;
	struct perf_addr_filter *filter;

	list_for_each_entry(filter, filters, entry) {
		/*
		 * No need to go further if there's no more
		 * room for filters.
		 */
		if (++index > ETM_ADDR_CMP_MAX)
			return -EOPNOTSUPP;

		/*
		 * As taken from the struct perf_addr_filter documentation:
		 *	@range:	1: range, 0: address
		 *
		 * At this time we don't allow range and start/stop filtering
		 * to cohabitate, they have to be mutually exclusive.
		 */
		if ((filter->range == 1) && address)
			return -EOPNOTSUPP;

		if ((filter->range == 0) && range)
			return -EOPNOTSUPP;

		/*
		 * For range filtering, the second address in the address
		 * range comparator needs to be higher than the first.
		 * Invalid otherwise.
		 */
		if (filter->range && filter->size == 0)
			return -EINVAL;

		/*
		 * Everything checks out with this filter, record what we've
		 * received before moving on to the next one.
		 */
		if (filter->range)
			range = true;
		else
			address = true;
	}

	return 0;
}
