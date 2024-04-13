map_layer_by_name(const char* name)
{
	int i;

	for (i = 0; i < s_map->num_layers; ++i) {
		if (strcmp(name, lstr_cstr(s_map->layers[0].name)) == 0)
			return i;
	}
	return -1;
}
