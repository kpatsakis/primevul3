BGD_DECLARE(void *) gdDPExtractData (struct gdIOCtx *ctx, int *size)
{
	dynamicPtr *dp;
	dpIOCtx *dctx;
	void *data;

	dctx = (dpIOCtx *)ctx;
	dp = dctx->dp;

	/* clean up the data block and return it */
	if(dp->dataGood) {
		trimDynamic(dp);
		*size = dp->logicalSize;
		data = dp->data;
	} else {
		*size = 0;
		data = NULL;
		/* 2.0.21: never free memory we don't own */
		if((dp->data != NULL) && (dp->freeOK)) {
			gdFree(dp->data);
		}
	}

	dp->data = NULL;
	dp->realSize = 0;
	dp->logicalSize = 0;

	return data;
}
