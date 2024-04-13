void * gdImageWebpPtrEx (gdImagePtr im, int *size, int quantization)
{
	void *rv;
	gdIOCtx *out = gdNewDynamicCtx(2048, NULL);
	gdImageWebpCtx(im, out, quantization);
	rv = gdDPExtractData(out, size);
	out->gd_free(out);
	return rv;
}
