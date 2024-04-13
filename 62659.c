dispose(GObject *obj)
{
	RSFilter *filter = RS_FILTER(obj);

	if (!filter->dispose_has_run)
	{
		filter->dispose_has_run = TRUE;
		if (filter->previous)
		{
			filter->previous->next_filters = g_slist_remove(filter->previous->next_filters, filter);
			g_object_unref(filter->previous);
		}
	}
}
