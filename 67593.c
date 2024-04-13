static void nsc_profiler_print(NSC_CONTEXT_PRIV* priv)
{
	PROFILER_PRINT_HEADER
	PROFILER_PRINT(priv->prof_nsc_rle_decompress_data)
	PROFILER_PRINT(priv->prof_nsc_decode)
	PROFILER_PRINT(priv->prof_nsc_rle_compress_data)
	PROFILER_PRINT(priv->prof_nsc_encode)
	PROFILER_PRINT_FOOTER
}
