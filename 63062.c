zone_set_layer(int zone_index, int layer)
{
	struct map_zone* zone;

	zone = vector_get(s_map->zones, zone_index);
	zone->layer = layer;
}
