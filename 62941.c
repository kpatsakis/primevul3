layer_replace_tiles(int layer, int old_index, int new_index)
{
	int              layer_h;
	int              layer_w;
	struct map_tile* tile;

	int i_x, i_y;

	layer_w = s_map->layers[layer].width;
	layer_h = s_map->layers[layer].height;
	for (i_x = 0; i_x < layer_w; ++i_x) for (i_y = 0; i_y < layer_h; ++i_y) {
		tile = &s_map->layers[layer].tilemap[i_x + i_y * layer_w];
		if (tile->tile_index == old_index)
			tile->tile_index = new_index;
	}
}
