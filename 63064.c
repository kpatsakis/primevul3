zone_set_steps(int zone_index, int interval)
{
	struct map_zone* zone;

	zone = vector_get(s_map->zones, zone_index);
	zone->interval = interval;
	zone->steps_left = 0;
}
