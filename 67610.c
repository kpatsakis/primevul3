static void gdi_Glyph_Free(rdpContext* context, rdpGlyph* glyph)
{
	gdiGlyph* gdi_glyph;
	gdi_glyph = (gdiGlyph*) glyph;

	if (gdi_glyph)
	{
		gdi_SelectObject(gdi_glyph->hdc, (HGDIOBJECT) gdi_glyph->org_bitmap);
		gdi_DeleteObject((HGDIOBJECT) gdi_glyph->bitmap);
		gdi_DeleteDC(gdi_glyph->hdc);
		free(glyph->aj);
		free(glyph);
	}
}
