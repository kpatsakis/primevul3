static BOOL gdi_Bitmap_SetSurface(rdpContext* context, rdpBitmap* bitmap,
                                  BOOL primary)
{
	rdpGdi* gdi;

	if (!context)
		return FALSE;

	gdi = context->gdi;

	if (!gdi)
		return FALSE;

	if (primary)
		gdi->drawing = gdi->primary;
	else
		gdi->drawing = (gdiBitmap*) bitmap;

	return TRUE;
}
