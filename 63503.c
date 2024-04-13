static bool mappings_overlap(struct uid_gid_map *new_map,
			     struct uid_gid_extent *extent)
{
	u32 upper_first, lower_first, upper_last, lower_last;
	unsigned idx;

	upper_first = extent->first;
	lower_first = extent->lower_first;
	upper_last = upper_first + extent->count - 1;
	lower_last = lower_first + extent->count - 1;

	for (idx = 0; idx < new_map->nr_extents; idx++) {
		u32 prev_upper_first, prev_lower_first;
		u32 prev_upper_last, prev_lower_last;
		struct uid_gid_extent *prev;

		if (new_map->nr_extents <= UID_GID_MAP_MAX_BASE_EXTENTS)
			prev = &new_map->extent[idx];
		else
			prev = &new_map->forward[idx];

		prev_upper_first = prev->first;
		prev_lower_first = prev->lower_first;
		prev_upper_last = prev_upper_first + prev->count - 1;
		prev_lower_last = prev_lower_first + prev->count - 1;

		/* Does the upper range intersect a previous extent? */
		if ((prev_upper_first <= upper_last) &&
		    (prev_upper_last >= upper_first))
			return true;

		/* Does the lower range intersect a previous extent? */
		if ((prev_lower_first <= lower_last) &&
		    (prev_lower_last >= lower_first))
			return true;
	}
	return false;
}
