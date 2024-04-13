map_trigger_at(int x, int y, int layer)
{
	rect_t              bounds;
	int                 tile_w, tile_h;
	struct map_trigger* trigger;

	iter_t iter;

	tileset_get_size(s_map->tileset, &tile_w, &tile_h);
	iter = vector_enum(s_map->triggers);
	while ((trigger = iter_next(&iter))) {
		if (trigger->z != layer && false)  // layer ignored for compatibility
			continue;
		bounds.x1 = trigger->x - tile_w / 2;
		bounds.y1 = trigger->y - tile_h / 2;
		bounds.x2 = bounds.x1 + tile_w;
		bounds.y2 = bounds.y1 + tile_h;
		if (is_point_in_rect(x, y, bounds))
			return iter.index;
	}
	return -1;
}
