rs_filter_get_image8(RSFilter *filter, const RSFilterRequest *request)
{
	g_return_val_if_fail(RS_IS_FILTER(filter), NULL);
	g_return_val_if_fail(RS_IS_FILTER_REQUEST(request), NULL);

	RS_DEBUG(FILTERS, "rs_filter_get_image8(%s [%p])", RS_FILTER_NAME(filter), filter);

	/* This timer-hack will break badly when multithreaded! */
	static gfloat last_elapsed = 0.0;
	static gint count = -1;
	gfloat elapsed, temp;
	static GTimer *gt = NULL;

	RSFilterResponse *response = NULL;
	GdkPixbuf *image = NULL;
	GdkRectangle* roi = NULL;
	RSFilterRequest *r = NULL;

	g_return_val_if_fail(RS_IS_FILTER(filter), NULL);
	g_return_val_if_fail(RS_IS_FILTER_REQUEST(request), NULL);

	if (count == -1)
		gt = g_timer_new();
	count++;

	if (filter->enabled && (roi = rs_filter_request_get_roi(request)))
	{
		roi = clamp_roi(roi, filter, request);
		if (roi)
		{
			r = rs_filter_request_clone(request);
			rs_filter_request_set_roi(r, roi);
			request = r;
		}
	}

	if (RS_FILTER_GET_CLASS(filter)->get_image8 && filter->enabled)
		response = RS_FILTER_GET_CLASS(filter)->get_image8(filter, request);
	else if (filter->previous)
		response = rs_filter_get_image8(filter->previous, request);

	g_assert(RS_IS_FILTER_RESPONSE(response));

	image = rs_filter_response_get_image8(response);
	elapsed = g_timer_elapsed(gt, NULL) - last_elapsed;

	/* Subtract 16 bit time */
	if (rs_filter_param_get_float(RS_FILTER_PARAM(response), "16-bit-time", &temp))
		elapsed -= temp;

	if (roi)
		g_free(roi);
	if (r)
		g_object_unref(r);

#ifdef FILTER_SHOW_PERFORMANCE
	if ((elapsed > FILTER_PERF_ELAPSED_MIN) && (image != NULL)) {
		gint iw = gdk_pixbuf_get_width(image);
		gint ih = gdk_pixbuf_get_height(image);
		if (rs_filter_response_get_roi(response)) 
		{
			GdkRectangle *roi = rs_filter_response_get_roi(response);
			iw = roi->width;
			ih = roi->height;
		}
		filter_performance("%s took: \033[32m%.0f\033[0mms", RS_FILTER_NAME(filter), elapsed * 1000);
		filter_performance(" [\033[33m%.01f\033[0mMpix/s]", ((gfloat)(iw * ih)) / elapsed / 1000000.0);
		filter_performance("\n");
	}
#endif

	last_elapsed += elapsed;

	g_assert(GDK_IS_PIXBUF(image) || (image == NULL));

	count--;
	if (count == -1)
	{
		last_elapsed = 0.0;
		rs_filter_param_get_float(RS_FILTER_PARAM(response), "16-bit-time", &last_elapsed);
		last_elapsed = g_timer_elapsed(gt, NULL)-last_elapsed;
		if (last_elapsed > CHAIN_PERF_ELAPSED_MIN)
			filter_performance("Complete 8 bit chain took: \033[32m%.0f\033[0mms\n\n", last_elapsed*1000.0);
		g_timer_destroy(gt);
		last_elapsed = 0.0;
	}

	if (image)
		g_object_unref(image);

	return response;
}
