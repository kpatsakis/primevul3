trigger_activate(int trigger_index)
{
	int                 last_trigger;
	struct map_trigger* trigger;

	trigger = vector_get(s_map->triggers, trigger_index);
	last_trigger = s_current_trigger;
	s_current_trigger = trigger_index;
	script_run(trigger->script, true);
	s_current_trigger = last_trigger;
}
