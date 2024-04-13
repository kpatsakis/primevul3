free_map(struct map* map)
{
	struct map_trigger* trigger;
	struct map_zone*    zone;

	iter_t iter;
	int    i;

	if (map == NULL)
		return;
	for (i = 0; i < MAP_SCRIPT_MAX; ++i)
		script_unref(map->scripts[i]);
	for (i = 0; i < map->num_layers; ++i) {
		script_unref(map->layers[i].render_script);
		lstr_free(map->layers[i].name);
		free(map->layers[i].tilemap);
		obsmap_free(map->layers[i].obsmap);
	}
	for (i = 0; i < map->num_persons; ++i) {
		lstr_free(map->persons[i].name);
		lstr_free(map->persons[i].spriteset);
		lstr_free(map->persons[i].create_script);
		lstr_free(map->persons[i].destroy_script);
		lstr_free(map->persons[i].command_script);
		lstr_free(map->persons[i].talk_script);
		lstr_free(map->persons[i].touch_script);
	}
	iter = vector_enum(s_map->triggers);
	while ((trigger = iter_next(&iter)))
		script_unref(trigger->script);
	iter = vector_enum(s_map->zones);
	while ((zone = iter_next(&iter)))
		script_unref(zone->script);
	lstr_free(s_map->bgm_file);
	tileset_free(map->tileset);
	free(map->layers);
	free(map->persons);
	vector_free(map->triggers);
	vector_free(map->zones);
	free(map);
}
