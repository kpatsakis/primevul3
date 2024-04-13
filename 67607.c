static BOOL gdi_Glyph_BeginDraw(rdpContext* context, INT32 x, INT32 y,
                                INT32 width, INT32 height, UINT32 bgcolor,
                                UINT32 fgcolor, BOOL fOpRedundant)
{
	rdpGdi* gdi;

	if (!context || !context->gdi)
		return FALSE;

	gdi = context->gdi;

	if (!gdi->drawing || !gdi->drawing->hdc)
		return FALSE;

	if (!fOpRedundant)
	{
		if (!gdi_decode_color(gdi, bgcolor, &bgcolor, NULL))
			return FALSE;

		if (!gdi_decode_color(gdi, fgcolor, &fgcolor, NULL))
			return FALSE;

		gdi_SetClipRgn(gdi->drawing->hdc, x, y, width, height);
		gdi_SetTextColor(gdi->drawing->hdc, bgcolor);
		gdi_SetBkColor(gdi->drawing->hdc, fgcolor);

		if (1)
		{
			GDI_RECT rect = { 0 };
			HGDI_BRUSH brush = gdi_CreateSolidBrush(fgcolor);

			if (!brush)
				return FALSE;

			if (x > 0)
				rect.left = x;

			if (y > 0)
				rect.top = y;

			rect.right = x + width - 1;
			rect.bottom = y + height - 1;

			if ((x + width > rect.left) && (y + height > rect.top))
				gdi_FillRect(gdi->drawing->hdc, &rect, brush);

			gdi_DeleteObject((HGDIOBJECT)brush);
		}

		return gdi_SetNullClipRgn(gdi->drawing->hdc);
	}

	return TRUE;
}
