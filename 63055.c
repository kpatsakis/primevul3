update_map_engine(bool in_main_loop)
{
	bool                has_moved;
	int                 index;
	bool                is_sort_needed = false;
	int                 last_trigger;
	int                 last_zone;
	int                 layer;
	int                 map_w, map_h;
	int                 num_zone_steps;
	script_t*           script_to_run;
	int                 script_type;
	double              start_x[PLAYER_MAX];
	double              start_y[PLAYER_MAX];
	int                 tile_w, tile_h;
	struct map_trigger* trigger;
	double              x, y, px, py;
	struct map_zone*    zone;

	int i, j, k;

	++s_frames;
	tileset_get_size(s_map->tileset, &tile_w, &tile_h);
	map_w = s_map->width * tile_w;
	map_h = s_map->height * tile_h;

	tileset_update(s_map->tileset);

	for (i = 0; i < PLAYER_MAX; ++i) if (s_players[i].person != NULL)
		person_get_xy(s_players[i].person, &start_x[i], &start_y[i], false);
	for (i = 0; i < s_num_persons; ++i) {
		if (s_persons[i]->leader != NULL)
			continue;  // skip followers for now
		update_person(s_persons[i], &has_moved);
		is_sort_needed |= has_moved;
	}
	if (is_sort_needed)
		sort_persons();

	if (s_fade_progress < s_fade_frames) {
		++s_fade_progress;
		s_color_mask = color_mix(s_fade_color_to, s_fade_color_from,
			s_fade_progress, s_fade_frames - s_fade_progress);
	}

	if (s_camera_person != NULL) {
		person_get_xy(s_camera_person, &x, &y, true);
		s_camera_x = x; s_camera_y = y;
	}

	if (in_main_loop && !s_map->is_repeating) {
		script_type = s_camera_y < 0 ? MAP_SCRIPT_ON_LEAVE_NORTH
			: s_camera_x >= map_w ? MAP_SCRIPT_ON_LEAVE_EAST
			: s_camera_y >= map_h ? MAP_SCRIPT_ON_LEAVE_SOUTH
			: s_camera_x < 0 ? MAP_SCRIPT_ON_LEAVE_WEST
			: MAP_SCRIPT_MAX;
		if (script_type < MAP_SCRIPT_MAX)
			map_activate(script_type, true);
	}

	for (i = 0; i < PLAYER_MAX; ++i) if (s_players[i].person != NULL) {
		person_get_xyz(s_players[i].person, &x, &y, &layer, true);
		trigger = get_trigger_at(x, y, layer, &index);
		if (trigger != s_on_trigger) {
			last_trigger = s_current_trigger;
			s_current_trigger = index;
			s_on_trigger = trigger;
			if (trigger != NULL)
				script_run(trigger->script, false);
			s_current_trigger = last_trigger;
		}
	}

	for (k = 0; k < PLAYER_MAX; ++k) if (s_players[k].person != NULL) {
		person_get_xy(s_players[k].person, &x, &y, false);
		px = fabs(x - start_x[k]);
		py = fabs(y - start_y[k]);
		num_zone_steps = px > py ? px : py;
		for (i = 0; i < num_zone_steps; ++i) {
			j = 0;
			while ((zone = get_zone_at(x, y, layer, j++, &index))) {
				if (zone->steps_left-- <= 0) {
					last_zone = s_current_zone;
					s_current_zone = index;
					zone->steps_left = zone->interval;
					script_run(zone->script, true);
					s_current_zone = last_zone;
				}
			}
		}
	}

	for (i = 0; i < s_num_deferreds; ++i) {
		if (s_deferreds[i].frames_left-- <= 0) {
			script_to_run = s_deferreds[i].script;
			for (j = i; j < s_num_deferreds - 1; ++j)
				s_deferreds[j] = s_deferreds[j + 1];
			--s_num_deferreds;
			script_run(script_to_run, false);
			script_unref(script_to_run);
			--i;
		}
	}

	script_run(s_update_script, false);
}
