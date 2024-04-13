static u32 map_id_down(struct uid_gid_map *map, u32 id)
{
	return map_id_range_down(map, id, 1);
}
