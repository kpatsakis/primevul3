map_engine_on_render(script_t* script)
{
	script_unref(s_render_script);
	s_render_script = script_ref(script);
}
