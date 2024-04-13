map_id_range_down_max(unsigned extents, struct uid_gid_map *map, u32 id, u32 count)
{
	struct idmap_key key;

	key.map_up = false;
	key.count = count;
	key.id = id;

	return bsearch(&key, map->forward, extents,
		       sizeof(struct uid_gid_extent), cmp_map_id);
}
