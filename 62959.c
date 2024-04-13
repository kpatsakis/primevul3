map_engine_fade_to(color_t color_mask, int num_frames)
{
	if (num_frames > 0) {
		s_fade_color_to = color_mask;
		s_fade_color_from = s_color_mask;
		s_fade_frames = num_frames;
		s_fade_progress = 0;
	}
	else {
		s_color_mask = color_mask;
		s_fade_color_to = s_fade_color_from = color_mask;
		s_fade_progress = s_fade_frames = 0;
	}
}
