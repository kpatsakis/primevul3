BOOL nsc_write_message(NSC_CONTEXT* context, wStream* s, NSC_MESSAGE* message)
{
	UINT32 totalPlaneByteCount;
	totalPlaneByteCount = message->LumaPlaneByteCount +
	                      message->OrangeChromaPlaneByteCount +
	                      message->GreenChromaPlaneByteCount + message->AlphaPlaneByteCount;

	if (!Stream_EnsureRemainingCapacity(s, 20 + totalPlaneByteCount))
		return -1;

	Stream_Write_UINT32(s,
	                    message->LumaPlaneByteCount); /* LumaPlaneByteCount (4 bytes) */
	Stream_Write_UINT32(s,
	                    message->OrangeChromaPlaneByteCount); /* OrangeChromaPlaneByteCount (4 bytes) */
	Stream_Write_UINT32(s,
	                    message->GreenChromaPlaneByteCount); /* GreenChromaPlaneByteCount (4 bytes) */
	Stream_Write_UINT32(s,
	                    message->AlphaPlaneByteCount); /* AlphaPlaneByteCount (4 bytes) */
	Stream_Write_UINT8(s, message->ColorLossLevel); /* ColorLossLevel (1 byte) */
	Stream_Write_UINT8(s,
	                   message->ChromaSubsamplingLevel); /* ChromaSubsamplingLevel (1 byte) */
	Stream_Write_UINT16(s, 0); /* Reserved (2 bytes) */

	if (message->LumaPlaneByteCount)
		Stream_Write(s, message->PlaneBuffers[0],
		             message->LumaPlaneByteCount); /* LumaPlane */

	if (message->OrangeChromaPlaneByteCount)
		Stream_Write(s, message->PlaneBuffers[1],
		             message->OrangeChromaPlaneByteCount); /* OrangeChromaPlane */

	if (message->GreenChromaPlaneByteCount)
		Stream_Write(s, message->PlaneBuffers[2],
		             message->GreenChromaPlaneByteCount); /* GreenChromaPlane */

	if (message->AlphaPlaneByteCount)
		Stream_Write(s, message->PlaneBuffers[3],
		             message->AlphaPlaneByteCount); /* AlphaPlane */

	return TRUE;
}
