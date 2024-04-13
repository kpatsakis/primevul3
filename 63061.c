zone_set_bounds(int zone_index, rect_t bounds)
{
	struct map_zone* zone;

	zone = vector_get(s_map->zones, zone_index);
	rect_normalize(&bounds);
	zone->bounds = bounds;
}
