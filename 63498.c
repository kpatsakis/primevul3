map_id_range_down_base(unsigned extents, struct uid_gid_map *map, u32 id, u32 count)
{
	unsigned idx;
	u32 first, last, id2;

	id2 = id + count - 1;

	/* Find the matching extent */
	for (idx = 0; idx < extents; idx++) {
		first = map->extent[idx].first;
		last = first + map->extent[idx].count - 1;
		if (id >= first && id <= last &&
		    (id2 >= first && id2 <= last))
			return &map->extent[idx];
	}
	return NULL;
}
