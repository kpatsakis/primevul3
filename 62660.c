rs_filter_changed(RSFilter *filter, RSFilterChangedMask mask)
{
	RS_DEBUG(FILTERS, "rs_filter_changed(%s [%p], %04x)", RS_FILTER_NAME(filter), filter, mask);
	g_return_if_fail(RS_IS_FILTER(filter));

	gint i, n_next = g_slist_length(filter->next_filters);

	for(i=0; i<n_next; i++)
	{
		RSFilter *next = RS_FILTER(g_slist_nth_data(filter->next_filters, i));

		g_assert(RS_IS_FILTER(next));

		/* Notify "next" filter or try "next next" filter */
		if (RS_FILTER_GET_CLASS(next)->previous_changed)
			RS_FILTER_GET_CLASS(next)->previous_changed(next, filter, mask);
		else
			rs_filter_changed(next, mask);
	}

	g_signal_emit(G_OBJECT(filter), signals[CHANGED_SIGNAL], 0, mask);
}
