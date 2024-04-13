rs_filter_set_enabled(RSFilter *filter, gboolean enabled)
{
	gboolean previous_state;

	g_return_val_if_fail(RS_IS_FILTER(filter), FALSE);

	previous_state = filter->enabled;

	if (filter->enabled != enabled)
	{
		filter->enabled = enabled;
		rs_filter_changed(filter, RS_FILTER_CHANGED_PIXELDATA);
	}

	return previous_state;
}
