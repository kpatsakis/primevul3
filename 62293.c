static toff_t tiff_sizeproc(thandle_t clientdata)
{
	tiff_handle *th = (tiff_handle *)clientdata;
	return th->size;
}
