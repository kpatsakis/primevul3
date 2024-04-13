map_screen_to_layer(int layer, int camera_x, int camera_y, int* inout_x, int* inout_y)
{
	rect_t  bounds;
	int     center_x;
	int     center_y;
	int     layer_h;
	int     layer_w;
	float   plx_offset_x = 0.0;
	int     plx_offset_y = 0.0;
	size2_t resolution;
	int     tile_w;
	int     tile_h;
	int     x_offset;
	int     y_offset;

	resolution = screen_size(g_screen);
	tileset_get_size(s_map->tileset, &tile_w, &tile_h);
	layer_w = s_map->layers[layer].width * tile_w;
	layer_h = s_map->layers[layer].height * tile_h;
	center_x = resolution.width / 2;
	center_y = resolution.height / 2;

	if (!s_map->is_repeating) {
		bounds = map_bounds();
		camera_x = fmin(fmax(camera_x, bounds.x1 + center_x), bounds.x2 - center_x);
		camera_y = fmin(fmax(camera_y, bounds.y1 + center_y), bounds.y2 - center_y);
	}

	plx_offset_x = s_frames * s_map->layers[layer].autoscroll_x
		- camera_x * (s_map->layers[layer].parallax_x - 1.0);
	plx_offset_y = s_frames * s_map->layers[layer].autoscroll_y
		- camera_y * (s_map->layers[layer].parallax_y - 1.0);
	x_offset = camera_x - center_x - plx_offset_x;
	y_offset = camera_y - center_y - plx_offset_y;
	if (!s_map->is_repeating && !s_map->layers[layer].is_parallax) {
		if (layer_w < resolution.width)
			x_offset = 0;
		if (layer_h < resolution.height)
			y_offset = 0;
	}
	if (inout_x != NULL)
		*inout_x += x_offset;
	if (inout_y != NULL)
		*inout_y += y_offset;

	if (s_map->is_repeating || s_map->layers[layer].is_parallax) {
		if (inout_x) *inout_x = (*inout_x % layer_w + layer_w) % layer_w;
		if (inout_y) *inout_y = (*inout_y % layer_h + layer_h) % layer_h;
	}
}
