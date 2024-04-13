int zgfx_compress(ZGFX_CONTEXT* zgfx, const BYTE* pSrcData, UINT32 SrcSize, BYTE** ppDstData,
                  UINT32* pDstSize, UINT32* pFlags)
{
	int status;
	wStream* s = Stream_New(NULL, SrcSize);
	status = zgfx_compress_to_stream(zgfx, s, pSrcData, SrcSize, pFlags);
	(*ppDstData) = Stream_Buffer(s);
	(*pDstSize) = Stream_GetPosition(s);
	Stream_Free(s, FALSE);
	return status;
}
