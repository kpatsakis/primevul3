void nsc_context_free(NSC_CONTEXT* context)
{
	size_t i;

	if (!context)
		return;

	if (context->priv)
	{
		for (i = 0; i < 4; i++)
			free(context->priv->PlaneBuffers[i]);

		BufferPool_Free(context->priv->PlanePool);
		nsc_profiler_print(context->priv);
		PROFILER_FREE(context->priv->prof_nsc_rle_decompress_data)
		PROFILER_FREE(context->priv->prof_nsc_decode)
		PROFILER_FREE(context->priv->prof_nsc_rle_compress_data)
		PROFILER_FREE(context->priv->prof_nsc_encode)
		free(context->priv);
	}

	free(context->BitmapData);
	free(context);
}
