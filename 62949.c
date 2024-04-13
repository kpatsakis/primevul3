map_bounds(void)
{
	rect_t bounds;
	int    tile_w, tile_h;

	tileset_get_size(s_map->tileset, &tile_w, &tile_h);
	bounds.x1 = 0; bounds.y1 = 0;
	bounds.x2 = s_map->width * tile_w;
	bounds.y2 = s_map->height * tile_h;
	return bounds;
}
