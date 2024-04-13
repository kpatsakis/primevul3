static void check_and_drop(void *_data)
{
	struct detach_data *data = _data;

	if (!data->mountpoint && !data->select.found)
		__d_drop(data->select.start);
}
