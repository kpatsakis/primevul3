 static BOOL nsc_stream_initialize(NSC_CONTEXT* context, wStream* s)
{
	int i;

	if (Stream_GetRemainingLength(s) < 20)
		return FALSE;

	for (i = 0; i < 4; i++)
		Stream_Read_UINT32(s, context->PlaneByteCount[i]);

	Stream_Read_UINT8(s, context->ColorLossLevel); /* ColorLossLevel (1 byte) */
	Stream_Read_UINT8(s,
	                  context->ChromaSubsamplingLevel); /* ChromaSubsamplingLevel (1 byte) */
	Stream_Seek(s, 2); /* Reserved (2 bytes) */
	context->Planes = Stream_Pointer(s);
	return TRUE;
}
