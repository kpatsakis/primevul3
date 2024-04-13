layer_on_render(int layer, script_t* script)
{
	script_unref(s_map->layers[layer].render_script);
	s_map->layers[layer].render_script = script_ref(script);
}
