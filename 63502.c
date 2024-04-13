map_id_up_max(unsigned extents, struct uid_gid_map *map, u32 id)
{
	struct idmap_key key;

	key.map_up = true;
	key.count = 1;
	key.id = id;

	return bsearch(&key, map->reverse, extents,
		       sizeof(struct uid_gid_extent), cmp_map_id);
}
