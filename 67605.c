static BOOL gdi_Bitmap_Paint(rdpContext* context, rdpBitmap* bitmap)
{
	gdiBitmap* gdi_bitmap = (gdiBitmap*) bitmap;
	UINT32 width = bitmap->right - bitmap->left + 1;
	UINT32 height = bitmap->bottom - bitmap->top + 1;
	return gdi_BitBlt(context->gdi->primary->hdc,
	                  bitmap->left, bitmap->top,
	                  width, height, gdi_bitmap->hdc,
	                  0, 0, GDI_SRCCOPY, &context->gdi->palette);
}
