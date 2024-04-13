static u32 map_id_range_down(struct uid_gid_map *map, u32 id, u32 count)
{
	struct uid_gid_extent *extent;
	unsigned extents = map->nr_extents;
	smp_rmb();

	if (extents <= UID_GID_MAP_MAX_BASE_EXTENTS)
		extent = map_id_range_down_base(extents, map, id, count);
	else
		extent = map_id_range_down_max(extents, map, id, count);

	/* Map the id or note failure */
	if (extent)
		id = (id - extent->first) + extent->lower_first;
	else
		id = (u32) -1;

	return id;
}
