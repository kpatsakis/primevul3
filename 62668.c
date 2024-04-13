rs_filter_new(const gchar *name, RSFilter *previous)
{
	RS_DEBUG(FILTERS, "rs_filter_new(%s, %s [%p])", name, RS_FILTER_NAME(previous), previous);
	g_return_val_if_fail(name != NULL, NULL);
	g_return_val_if_fail((previous == NULL) || RS_IS_FILTER(previous), NULL);

	GType type = g_type_from_name(name);
	RSFilter *filter = NULL;

	if (g_type_is_a (type, RS_TYPE_FILTER))
		filter = g_object_new(type, NULL);

	if (!RS_IS_FILTER(filter))
		g_warning("Could not instantiate filter of type \"%s\"", name);

	if (previous)
		rs_filter_set_previous(filter, previous);

	return filter;
}
