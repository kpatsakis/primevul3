BGD_DECLARE(void *) gdImageTiffPtr(gdImagePtr im, int *size)
{
	void *rv;
	gdIOCtx *out = gdNewDynamicCtx (2048, NULL);
	if (out == NULL) return NULL;
	gdImageTiffCtx(im, out); /* what's an fg again? */
	rv = gdDPExtractData(out, size);
	out->gd_free(out);
	return rv;
}
