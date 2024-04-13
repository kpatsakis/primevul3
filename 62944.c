layer_set_tile(int layer, int x, int y, int tile_index)
{
	struct map_tile* tile;
	int              width;

	width = s_map->layers[layer].width;
	tile = &s_map->layers[layer].tilemap[x + y * width];
	tile->tile_index = tile_index;
	tile->frames_left = tileset_get_delay(s_map->tileset, tile_index);
}
