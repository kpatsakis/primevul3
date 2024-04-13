map_engine_defer(script_t* script, int num_frames)
{
	struct deferred* deferred;

	if (++s_num_deferreds > s_max_deferreds) {
		s_max_deferreds = s_num_deferreds * 2;
		s_deferreds = realloc(s_deferreds, s_max_deferreds * sizeof(struct deferred));
	}
	deferred = &s_deferreds[s_num_deferreds - 1];
	deferred->script = script;
	deferred->frames_left = num_frames;
}
