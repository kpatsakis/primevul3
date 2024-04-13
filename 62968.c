map_engine_on_person_event(person_op_t op, script_t* script)
{
	script_t* old_script;

	old_script = s_def_person_scripts[op];
	s_def_person_scripts[op] = script_ref(script);
	script_unref(old_script);
}
