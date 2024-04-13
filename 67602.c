 void nsc_init_sse2(NSC_CONTEXT* context)
{
	if (!IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
		return;

	PROFILER_RENAME(context->priv->prof_nsc_encode, "nsc_encode_sse2");
	context->encode = nsc_encode_sse2;
}
