zone_get_steps(int zone_index)
{
	struct map_zone* zone;

	zone = vector_get(s_map->zones, zone_index);
	return zone->interval;
}
