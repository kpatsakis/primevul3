static BOOL gdi_Glyph_New(rdpContext* context, const rdpGlyph* glyph)
{
	BYTE* data;
	gdiGlyph* gdi_glyph;

	if (!context || !glyph)
		return FALSE;

	gdi_glyph = (gdiGlyph*) glyph;
	gdi_glyph->hdc = gdi_GetDC();

	if (!gdi_glyph->hdc)
		return FALSE;

	gdi_glyph->hdc->format = PIXEL_FORMAT_MONO;
	data = freerdp_glyph_convert(glyph->cx, glyph->cy, glyph->aj);

	if (!data)
	{
		gdi_DeleteDC(gdi_glyph->hdc);
		return FALSE;
	}

	gdi_glyph->bitmap = gdi_CreateBitmap(glyph->cx, glyph->cy, PIXEL_FORMAT_MONO,
	                                     data);

	if (!gdi_glyph->bitmap)
	{
		gdi_DeleteDC(gdi_glyph->hdc);
		_aligned_free(data);
		return FALSE;
	}

	gdi_SelectObject(gdi_glyph->hdc, (HGDIOBJECT) gdi_glyph->bitmap);
	gdi_glyph->org_bitmap = NULL;
	return TRUE;
}
