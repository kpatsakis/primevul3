clamp_roi(const GdkRectangle *roi, RSFilter *filter, const RSFilterRequest *request)
{
	RSFilterResponse *response = rs_filter_get_size(filter, request);
	gint w = rs_filter_response_get_width(response);
	gint h = rs_filter_response_get_height(response);
	g_object_unref(response);

	if ((roi->x >= 0) && (roi->y >=0) && (roi->x + roi->width <= w) && (roi->y + roi->height <= h))
		return NULL;

	GdkRectangle* new_roi = g_new(GdkRectangle, 1);
	new_roi->x = MAX(0, roi->x);
	new_roi->y = MAX(0, roi->y);
	new_roi->width = MIN(w - new_roi->x, roi->width);
	new_roi->height = MAX(h - new_roi->y, roi->height);
	return new_roi;
}
