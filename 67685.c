ZGFX_CONTEXT* zgfx_context_new(BOOL Compressor)
{
	ZGFX_CONTEXT* zgfx;
	zgfx = (ZGFX_CONTEXT*) calloc(1, sizeof(ZGFX_CONTEXT));

	if (zgfx)
	{
		zgfx->Compressor = Compressor;
		zgfx->HistoryBufferSize = sizeof(zgfx->HistoryBuffer);
		zgfx_context_reset(zgfx, FALSE);
	}

	return zgfx;
}
