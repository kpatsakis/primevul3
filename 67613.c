HGDI_BITMAP gdi_create_bitmap(rdpGdi* gdi, UINT32 nWidth, UINT32 nHeight,
                              UINT32 SrcFormat, BYTE* data)
{
	UINT32 nSrcStep;
	UINT32 nDstStep;
	BYTE* pSrcData;
	BYTE* pDstData;
	HGDI_BITMAP bitmap;

	if (!gdi)
		return NULL;

	nDstStep = nWidth * GetBytesPerPixel(gdi->dstFormat);
	pDstData = _aligned_malloc(nHeight * nDstStep, 16);

	if (!pDstData)
		return NULL;

	pSrcData = data;
	nSrcStep = nWidth * GetBytesPerPixel(SrcFormat);

	if (!freerdp_image_copy(pDstData, gdi->dstFormat, nDstStep, 0, 0,
	                        nWidth, nHeight, pSrcData, SrcFormat, nSrcStep, 0, 0,
	                        &gdi->palette, FREERDP_FLIP_NONE))
	{
		_aligned_free(pDstData);
		return NULL;
	}

	bitmap = gdi_CreateBitmap(nWidth, nHeight, gdi->dstFormat, pDstData);
	return bitmap;
}
