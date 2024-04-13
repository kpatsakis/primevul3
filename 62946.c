map_activate(map_op_t op, bool use_default)
{
	if (use_default)
		script_run(s_def_map_scripts[op], false);
	script_run(s_map->scripts[op], false);
}
