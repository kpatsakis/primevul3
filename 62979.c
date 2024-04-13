map_engine_uninit(void)
{
	int i;

	console_log(1, "shutting down map engine subsystem");

	vector_free(s_person_list);

	for (i = 0; i < s_num_deferreds; ++i)
		script_unref(s_deferreds[i].script);
	free(s_deferreds);
	for (i = 0; i < MAP_SCRIPT_MAX; ++i)
		script_unref(s_def_map_scripts[i]);
	script_unref(s_update_script);
	script_unref(s_render_script);
	free_map(s_map);
	free(s_players);

	for (i = 0; i < s_num_persons; ++i)
		free_person(s_persons[i]);
	for (i = 0; i < PERSON_SCRIPT_MAX; ++i)
		script_unref(s_def_person_scripts[i]);
	free(s_persons);

	mixer_unref(s_bgm_mixer);

	audio_uninit();
}
