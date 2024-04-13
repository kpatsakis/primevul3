zone_activate(int zone_index)
{
	int              last_zone;
	struct map_zone* zone;

	zone = vector_get(s_map->zones, zone_index);
	last_zone = s_current_zone;
	s_current_zone = zone_index;
	script_run(zone->script, true);
	s_current_zone = last_zone;
}
