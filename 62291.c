static tsize_t tiff_readproc(thandle_t clientdata, tdata_t data, tsize_t size)
{
	tiff_handle *th = (tiff_handle *)clientdata;
	gdIOCtx *ctx = th->ctx;

	size = (ctx->getBuf)(ctx, data, size);

	return size;
}
