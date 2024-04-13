void Clipboard::WriteBitmap(const char* pixel_data, const char* size_data) {
  const gfx::Size* size = reinterpret_cast<const gfx::Size*>(size_data);

  guchar* data =
      gfx::BGRAToRGBA(reinterpret_cast<const uint8_t*>(pixel_data),
                      size->width(), size->height(), 0);

  GdkPixbuf* pixbuf =
      gdk_pixbuf_new_from_data(data, GDK_COLORSPACE_RGB, TRUE,
                               8, size->width(), size->height(),
                               size->width() * 4, GdkPixbufFree, NULL);
  InsertMapping(kMimeTypeBitmap, reinterpret_cast<char*>(pixbuf), 0);
}
