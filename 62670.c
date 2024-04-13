rs_filter_set_label(RSFilter *filter, const gchar *label)
{
	g_return_if_fail(RS_IS_FILTER(filter));

	filter->label = label;	
}
