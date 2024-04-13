BGD_DECLARE(gdImagePtr) gdImageCreateFromTiffPtr(int size, void *data)
{
	gdImagePtr im;
	gdIOCtx *in = gdNewDynamicCtxEx (size, data, 0);
	if (in == NULL) return NULL;
	im = gdImageCreateFromTiffCtx(in);
	in->gd_free(in);
	return im;
}
