rs_filter_get_label(RSFilter *filter)
{
	g_return_val_if_fail(RS_IS_FILTER(filter), "");

	return filter->label;
}
