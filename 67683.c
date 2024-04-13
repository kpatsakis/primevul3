static BOOL zgfx_compress_segment(ZGFX_CONTEXT* zgfx, wStream* s, const BYTE* pSrcData,
                                  UINT32 SrcSize, UINT32* pFlags)
{
	/* FIXME: Currently compression not implemented. Just copy the raw source */
	if (!Stream_EnsureRemainingCapacity(s, SrcSize + 1))
	{
		WLog_ERR(TAG, "Stream_EnsureRemainingCapacity failed!");
		return FALSE;
	}

	(*pFlags) |= ZGFX_PACKET_COMPR_TYPE_RDP8; /* RDP 8.0 compression format */
	Stream_Write_UINT8(s, (*pFlags)); /* header (1 byte) */
	Stream_Write(s, pSrcData, SrcSize);
	return TRUE;
}
