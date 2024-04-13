static int sort_idmaps(struct uid_gid_map *map)
{
	if (map->nr_extents <= UID_GID_MAP_MAX_BASE_EXTENTS)
		return 0;

	/* Sort forward array. */
	sort(map->forward, map->nr_extents, sizeof(struct uid_gid_extent),
	     cmp_extents_forward, NULL);

	/* Only copy the memory from forward we actually need. */
	map->reverse = kmemdup(map->forward,
			       map->nr_extents * sizeof(struct uid_gid_extent),
			       GFP_KERNEL);
	if (!map->reverse)
		return -ENOMEM;

	/* Sort reverse array. */
	sort(map->reverse, map->nr_extents, sizeof(struct uid_gid_extent),
	     cmp_extents_reverse, NULL);

	return 0;
}
