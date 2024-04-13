static void update_free_window_state(WINDOW_STATE_ORDER* window_state)
{
	if (!window_state)
		return;

	free(window_state->titleInfo.string);
	window_state->titleInfo.string = NULL;
	free(window_state->windowRects);
	window_state->windowRects = NULL;
	free(window_state->visibilityRects);
	window_state->visibilityRects = NULL;
}
