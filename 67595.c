BOOL nsc_compose_message(NSC_CONTEXT* context, wStream* s, const BYTE* data,
                         UINT32 width, UINT32 height, UINT32 scanline)
{
	NSC_MESSAGE s_message = { 0 };
	NSC_MESSAGE* message = &s_message;
	context->width = width;
	context->height = height;

	if (!nsc_context_initialize_encode(context))
		return FALSE;

	/* ARGB to AYCoCg conversion, chroma subsampling and colorloss reduction */
	PROFILER_ENTER(context->priv->prof_nsc_encode)
	context->encode(context, data, scanline);
	PROFILER_EXIT(context->priv->prof_nsc_encode)
	/* RLE encode */
	PROFILER_ENTER(context->priv->prof_nsc_rle_compress_data)
	nsc_rle_compress_data(context);
	PROFILER_EXIT(context->priv->prof_nsc_rle_compress_data)
	message->PlaneBuffers[0] = context->priv->PlaneBuffers[0];
	message->PlaneBuffers[1] = context->priv->PlaneBuffers[1];
	message->PlaneBuffers[2] = context->priv->PlaneBuffers[2];
	message->PlaneBuffers[3] = context->priv->PlaneBuffers[3];
	message->LumaPlaneByteCount = context->PlaneByteCount[0];
	message->OrangeChromaPlaneByteCount = context->PlaneByteCount[1];
	message->GreenChromaPlaneByteCount = context->PlaneByteCount[2];
	message->AlphaPlaneByteCount = context->PlaneByteCount[3];
	message->ColorLossLevel = context->ColorLossLevel;
	message->ChromaSubsamplingLevel = context->ChromaSubsamplingLevel;
	return nsc_write_message(context, s, message);
}
