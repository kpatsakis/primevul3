map_normalize_xy(double* inout_x, double* inout_y, int layer)
{
	int tile_w, tile_h;
	int layer_w, layer_h;

	if (s_map == NULL)
		return;  // can't normalize if no map loaded
	if (!s_map->is_repeating && !s_map->layers[layer].is_parallax)
		return;
	tileset_get_size(s_map->tileset, &tile_w, &tile_h);
	layer_w = s_map->layers[layer].width * tile_w;
	layer_h = s_map->layers[layer].height * tile_h;
	if (inout_x)
		*inout_x = fmod(fmod(*inout_x, layer_w) + layer_w, layer_w);
	if (inout_y)
		*inout_y = fmod(fmod(*inout_y, layer_h) + layer_h, layer_h);
}
