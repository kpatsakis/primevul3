get_zone_at(int x, int y, int layer, int which, int* out_index)
{
	struct map_zone* found_item = NULL;
	struct map_zone* zone;

	iter_t iter;
	int    i;

	iter = vector_enum(s_map->zones); i = -1;
	while ((zone = iter_next(&iter))) {
		if (zone->layer != layer && false)  // layer ignored for compatibility
			continue;
		if (is_point_in_rect(x, y, zone->bounds) && which-- == 0) {
			found_item = zone;
			if (out_index) *out_index = (int)iter.index;
			break;
		}
	}
	return found_item;
}
