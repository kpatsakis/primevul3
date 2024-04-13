zone_get_layer(int zone_index)
{
	struct map_zone* zone;

	zone = vector_get(s_map->zones, zone_index);
	return zone->layer;
}
