map_engine_start(const char* filename, int framerate)
{
	s_is_map_running = true;
	s_exiting = false;
	s_color_mask = mk_color(0, 0, 0, 0);
	s_fade_color_to = s_fade_color_from = s_color_mask;
	s_fade_progress = s_fade_frames = 0;
	al_clear_to_color(al_map_rgba(0, 0, 0, 255));
	s_frame_rate = framerate;
	if (!change_map(filename, true))
		goto on_error;
	while (!s_exiting && jsal_vm_enabled()) {
		sphere_heartbeat(true, 1);

		update_map_engine(true);
		process_map_input();
		map_engine_draw_map();

		sphere_tick(1, false, s_frame_rate);
	}
	reset_persons(false);
	s_is_map_running = false;
	return true;

on_error:
	s_is_map_running = false;
	return false;
}
