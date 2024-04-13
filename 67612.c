static BOOL gdi_Glyph_SetBounds(rdpContext* context, INT32 x, INT32 y, INT32 width, INT32 height)
{
	rdpGdi* gdi;

	if (!context || !context->gdi)
		return FALSE;

	gdi = context->gdi;

	if (!gdi->drawing || !gdi->drawing->hdc)
		return FALSE;

	return gdi_SetClipRgn(gdi->drawing->hdc, x, y, width, height);
}
