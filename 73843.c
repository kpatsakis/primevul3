void * gdImageWebpPtr (gdImagePtr im, int *size)
{
	void *rv;
	gdIOCtx *out = gdNewDynamicCtx(2048, NULL);
	gdImageWebpCtx(im, out, -1);
	rv = gdDPExtractData(out, size);
	out->gd_free(out);

	return rv;
}
