layer_size(int layer)
{
	struct map_layer* layer_data;

	layer_data = &s_map->layers[layer];
	return mk_size2(layer_data->width, layer_data->height);
}
