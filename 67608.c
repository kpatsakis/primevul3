static BOOL gdi_Glyph_Draw(rdpContext* context, const rdpGlyph* glyph, INT32 x,
                           INT32 y, INT32 w, INT32 h, INT32 sx, INT32 sy, BOOL fOpRedundant)
{
	gdiGlyph* gdi_glyph;
	rdpGdi* gdi;
	HGDI_BRUSH brush;
	BOOL rc = FALSE;

	if (!context || !glyph)
		return FALSE;

	gdi = context->gdi;
	gdi_glyph = (gdiGlyph*) glyph;

	if (!fOpRedundant && 0)
	{
		GDI_RECT rect = { 0 };

		if (x > 0)
			rect.left = x;

		if (y > 0)
			rect.top = y;

		if (x + w > 0)
			rect.right = x + w - 1;

		if (y + h > 0)
			rect.bottom = y + h - 1;

		if ((rect.left < rect.right) && (rect.top < rect.bottom))
		{
			brush = gdi_CreateSolidBrush(gdi->drawing->hdc->bkColor);

			if (!brush)
				return FALSE;

			gdi_FillRect(gdi->drawing->hdc, &rect, brush);
			gdi_DeleteObject((HGDIOBJECT)brush);
		}
	}

	brush = gdi_CreateSolidBrush(gdi->drawing->hdc->textColor);

	if (!brush)
		return FALSE;

	gdi_SelectObject(gdi->drawing->hdc, (HGDIOBJECT)brush);
	rc = gdi_BitBlt(gdi->drawing->hdc, x, y, w, h, gdi_glyph->hdc, sx, sy,
	                GDI_GLYPH_ORDER, &context->gdi->palette);
	gdi_DeleteObject((HGDIOBJECT)brush);
	return rc;
}
