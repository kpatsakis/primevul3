static u32 map_id_up(struct uid_gid_map *map, u32 id)
{
	struct uid_gid_extent *extent;
	unsigned extents = map->nr_extents;
	smp_rmb();

	if (extents <= UID_GID_MAP_MAX_BASE_EXTENTS)
		extent = map_id_up_base(extents, map, id);
	else
		extent = map_id_up_max(extents, map, id);

	/* Map the id or note failure */
	if (extent)
		id = (id - extent->lower_first) + extent->first;
	else
		id = (u32) -1;

	return id;
}
