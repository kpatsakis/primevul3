static BOOL gdi_Glyph_EndDraw(rdpContext* context, INT32 x, INT32 y,
                              INT32 width, INT32 height, UINT32 bgcolor, UINT32 fgcolor)
{
	rdpGdi* gdi;

	if (!context || !context->gdi)
		return FALSE;

	gdi = context->gdi;

	if (!gdi->drawing || !gdi->drawing->hdc)
		return FALSE;

	gdi_SetNullClipRgn(gdi->drawing->hdc);
	return TRUE;
}
