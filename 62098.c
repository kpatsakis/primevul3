static long dynamicTell(struct gdIOCtx *ctx)
{
	dpIOCtx *dctx;

	dctx = (dpIOCtx *)ctx;
	return (dctx->dp->pos);
}
