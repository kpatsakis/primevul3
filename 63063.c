zone_set_script(int zone_index, script_t* script)
{
	script_t*        old_script;
	struct map_zone* zone;

	zone = vector_get(s_map->zones, zone_index);
	old_script = zone->script;
	zone->script = script_ref(script);
	script_unref(old_script);
}
