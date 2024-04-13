trigger_set_script(int trigger_index, script_t* script)
{
	script_t*           old_script;
	struct map_trigger* trigger;

	trigger = vector_get(s_map->triggers, trigger_index);
	old_script = trigger->script;
	trigger->script = script_ref(script);
	script_unref(old_script);
}
