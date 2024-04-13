rs_filter_get_size_simple(RSFilter *filter, const RSFilterRequest *request, gint *width, gint *height)
{
	gint w, h;
	RSFilterResponse *response;

	g_return_val_if_fail(RS_IS_FILTER(filter), FALSE);
	g_return_val_if_fail(RS_IS_FILTER_REQUEST(request), FALSE);

	response = rs_filter_get_size(filter, request);

	if (!RS_IS_FILTER_RESPONSE(response))
		return FALSE;

	w = rs_filter_response_get_width(response);
	h = rs_filter_response_get_height(response);
	if (width)
		*width = w;
	if (height)
		*height = h;

	g_object_unref(response);

	return ((w>0) && (h>0));
}
