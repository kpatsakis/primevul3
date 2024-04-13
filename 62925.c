change_map(const char* filename, bool preserve_persons)
{

	struct map*        map;
	person_t*          person;
	struct map_person* person_info;
	path_t*            path;
	spriteset_t*       spriteset = NULL;

	int i;

	console_log(2, "changing current map to '%s'", filename);

	map = load_map(filename);
	if (map == NULL) return false;
	if (s_map != NULL) {
		map_activate(MAP_SCRIPT_ON_LEAVE, true);
	}

	free_map(s_map); free(s_map_filename);
	for (i = 0; i < s_num_deferreds; ++i)
		script_unref(s_deferreds[i].script);
	s_num_deferreds = 0;
	s_map = map; s_map_filename = strdup(filename);
	reset_persons(preserve_persons);

	for (i = 0; i < s_map->num_persons; ++i) {
		person_info = &s_map->persons[i];
		path = game_full_path(g_game, lstr_cstr(person_info->spriteset), "spritesets", true);
		spriteset = spriteset_load(path_cstr(path));
		path_free(path);
		if (spriteset == NULL)
			goto on_error;
		if (!(person = person_new(lstr_cstr(person_info->name), spriteset, false, NULL)))
			goto on_error;
		spriteset_unref(spriteset);
		person_set_xyz(person, person_info->x, person_info->y, person_info->z);
		person_compile_script(person, PERSON_SCRIPT_ON_CREATE, person_info->create_script);
		person_compile_script(person, PERSON_SCRIPT_ON_DESTROY, person_info->destroy_script);
		person_compile_script(person, PERSON_SCRIPT_ON_TOUCH, person_info->touch_script);
		person_compile_script(person, PERSON_SCRIPT_ON_TALK, person_info->talk_script);
		person_compile_script(person, PERSON_SCRIPT_GENERATOR, person_info->command_script);

		person_activate(person, PERSON_SCRIPT_ON_CREATE, NULL, false);
	}

	s_camera_x = s_map->origin.x;
	s_camera_y = s_map->origin.y;

	if (s_map->bgm_file == NULL && s_map_bgm_stream != NULL) {
		sound_unref(s_map_bgm_stream);
		lstr_free(s_last_bgm_file);
		s_map_bgm_stream = NULL;
		s_last_bgm_file = NULL;
	}
	else if (s_map->bgm_file != NULL
		&& (s_last_bgm_file == NULL || lstr_cmp(s_map->bgm_file, s_last_bgm_file) != 0))
	{
		sound_unref(s_map_bgm_stream);
		lstr_free(s_last_bgm_file);
		s_last_bgm_file = lstr_dup(s_map->bgm_file);
		path = game_full_path(g_game, lstr_cstr(s_map->bgm_file), "sounds", true);
		if ((s_map_bgm_stream = sound_new(path_cstr(path)))) {
			sound_set_repeat(s_map_bgm_stream, true);
			sound_play(s_map_bgm_stream, s_bgm_mixer);
		}
		path_free(path);
	}

	map_activate(MAP_SCRIPT_ON_ENTER, true);

	s_frames = 0;
	return true;

on_error:
	spriteset_unref(spriteset);
	free_map(s_map);
	return false;
}
