static void gdi_Bitmap_Free(rdpContext* context, rdpBitmap* bitmap)
{
	gdiBitmap* gdi_bitmap = (gdiBitmap*) bitmap;

	if (gdi_bitmap)
	{
		if (gdi_bitmap->hdc)
			gdi_SelectObject(gdi_bitmap->hdc, (HGDIOBJECT) gdi_bitmap->org_bitmap);

		gdi_DeleteObject((HGDIOBJECT) gdi_bitmap->bitmap);
		gdi_DeleteDC(gdi_bitmap->hdc);
		_aligned_free(bitmap->data);
	}

	free(bitmap);
}
