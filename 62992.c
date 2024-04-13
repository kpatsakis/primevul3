map_screen_to_map(int camera_x, int camera_y, int* inout_x, int* inout_y)
{
	rect_t  bounds;
	int     center_x;
	int     center_y;
	int     map_h;
	int     map_w;
	size2_t resolution;
	int     tile_h;
	int     tile_w;
	int     x_offset;
	int     y_offset;

	resolution = screen_size(g_screen);
	tileset_get_size(s_map->tileset, &tile_w, &tile_h);
	map_w = s_map->width * tile_w;
	map_h = s_map->height * tile_h;
	center_x = resolution.width / 2;
	center_y = resolution.height / 2;

	if (!s_map->is_repeating) {
		bounds = map_bounds();
		camera_x = fmin(fmax(camera_x, bounds.x1 + center_x), bounds.x2 - center_x);
		camera_y = fmin(fmax(camera_y, bounds.y1 + center_y), bounds.y2 - center_y);
	}

	x_offset = camera_x - center_x;
	y_offset = camera_y - center_y;
	if (!s_map->is_repeating) {
		if (map_w < resolution.width)
			x_offset = 0;
		if (map_h < resolution.height)
			y_offset = 0;
	}
	if (inout_x != NULL)
		*inout_x += x_offset;
	if (inout_y != NULL)
		*inout_y += y_offset;

	if (s_map->is_repeating) {
		if (inout_x) *inout_x = (*inout_x % map_w + map_w) % map_w;
		if (inout_y) *inout_y = (*inout_y % map_h + map_h) % map_h;
	}
}
