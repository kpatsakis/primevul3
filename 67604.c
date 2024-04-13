static BOOL gdi_Bitmap_New(rdpContext* context, rdpBitmap* bitmap)
{
	gdiBitmap* gdi_bitmap;
	rdpGdi* gdi = context->gdi;
	gdi_bitmap = (gdiBitmap*) bitmap;
	gdi_bitmap->hdc = gdi_CreateCompatibleDC(gdi->hdc);

	if (!gdi_bitmap->hdc)
		return FALSE;

	if (!bitmap->data)
		gdi_bitmap->bitmap = gdi_CreateCompatibleBitmap(
		                         gdi->hdc, bitmap->width,
		                         bitmap->height);
	else
	{
		UINT32 format = bitmap->format;
		gdi_bitmap->bitmap = gdi_create_bitmap(gdi, bitmap->width,
		                                       bitmap->height,
		                                       format, bitmap->data);
	}

	if (!gdi_bitmap->bitmap)
	{
		gdi_DeleteDC(gdi_bitmap->hdc);
		return FALSE;
	}

	gdi_bitmap->hdc->format = gdi_bitmap->bitmap->format;
	gdi_SelectObject(gdi_bitmap->hdc, (HGDIOBJECT) gdi_bitmap->bitmap);
	gdi_bitmap->org_bitmap = NULL;
	return TRUE;
}
