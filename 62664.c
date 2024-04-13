rs_filter_get_recursive(RSFilter *filter, ...)
{
	va_list ap;
	gchar *property_name;
	gpointer property_ret;
	RSFilter *current_filter;

	g_return_if_fail(RS_IS_FILTER(filter));

	va_start(ap, filter);

	/* Loop through all properties */
	while ((property_name = va_arg(ap, gchar *)))
	{
		property_ret = va_arg(ap, gpointer);

		g_assert(property_ret != NULL);

		current_filter = filter;
		/* Iterate through all filter previous to filter */
		do {
			if (current_filter->enabled && g_object_class_find_property(G_OBJECT_GET_CLASS(current_filter), property_name))
			{
				g_object_get(current_filter, property_name, property_ret, NULL);
				break;
			}
		} while (RS_IS_FILTER(current_filter = current_filter->previous));
	}

	va_end(ap);
}
