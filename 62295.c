static tsize_t tiff_writeproc(thandle_t clientdata, tdata_t data, tsize_t size)
{
	tiff_handle *th = (tiff_handle *)clientdata;
	gdIOCtx *ctx = th->ctx;

	size = (ctx->putBuf)(ctx, data, size);
	if(size + th->pos>th->size) {
		th->size = size + th->pos;
		th->pos += size;
	}

	return size;
}
