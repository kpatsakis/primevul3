map_engine_init(void)
{
	int i;

	console_log(1, "initializing map engine subsystem");

	audio_init();
	s_bgm_mixer = mixer_new(44100, 16, 2);

	memset(s_def_map_scripts, 0, MAP_SCRIPT_MAX * sizeof(int));
	memset(s_def_person_scripts, 0, PERSON_SCRIPT_MAX * sizeof(int));
	s_map = NULL; s_map_filename = NULL;
	s_camera_person = NULL;
	s_players = calloc(PLAYER_MAX, sizeof(struct player));
	for (i = 0; i < PLAYER_MAX; ++i)
		s_players[i].is_talk_allowed = true;
	s_current_trigger = -1;
	s_current_zone = -1;
	s_render_script = NULL;
	s_update_script = NULL;
	s_num_deferreds = s_max_deferreds = 0;
	s_deferreds = NULL;
	s_talk_button = 0;
	s_is_map_running = false;
	s_color_mask = mk_color(0, 0, 0, 0);
	s_on_trigger = NULL;

	s_num_persons = s_max_persons = 0;
	s_persons = NULL;
	s_talk_distance = 8;
	s_acting_person = NULL;
	s_current_person = NULL;
}
