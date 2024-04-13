layer_get_tile(int layer, int x, int y)
{
	struct map_tile* tile;
	int              width;

	width = s_map->layers[layer].width;
	tile = &s_map->layers[layer].tilemap[x + y * width];
	return tile->tile_index;
}
