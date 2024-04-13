map_zone_at(int x, int y, int layer, int which)
{
	struct map_zone* zone;

	iter_t iter;

	iter = vector_enum(s_map->zones);
	while ((zone = iter_next(&iter))) {
		if (zone->layer != layer && false)  // layer ignored for compatibility
			continue;
		if (is_point_in_rect(x, y, zone->bounds) && --which < 0)
			return iter.index;
	}
	return -1;
}
