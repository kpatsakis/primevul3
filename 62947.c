map_add_trigger(int x, int y, int layer, script_t* script)
{
	struct map_trigger trigger;

	console_log(2, "creating trigger #%d on map '%s'", vector_len(s_map->triggers), s_map_filename);
	console_log(3, "    location: '%s' @ (%d,%d)", lstr_cstr(s_map->layers[layer].name), x, y);

	trigger.x = x; trigger.y = y;
	trigger.z = layer;
	trigger.script = script_ref(script);
	if (!vector_push(s_map->triggers, &trigger))
		return false;
	return true;
}
