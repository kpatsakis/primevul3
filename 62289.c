static int tiff_closeproc(thandle_t clientdata)
{
	(void)clientdata;
	/*tiff_handle *th = (tiff_handle *)clientdata;
	gdIOCtx *ctx = th->ctx;

	(ctx->gd_free)(ctx);*/

	return 0;
}
