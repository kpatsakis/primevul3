rs_filter_get_size(RSFilter *filter, const RSFilterRequest *request)
{
	RSFilterResponse *response = NULL;

	g_return_val_if_fail(RS_IS_FILTER(filter), NULL);
	g_return_val_if_fail(RS_IS_FILTER_REQUEST(request), NULL);

	if (RS_FILTER_GET_CLASS(filter)->get_size && filter->enabled)
		response = RS_FILTER_GET_CLASS(filter)->get_size(filter, request);
	else if (filter->previous)
		response = rs_filter_get_size(filter->previous, request);

	return response;
}
