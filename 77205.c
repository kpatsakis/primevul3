SkBitmap Clipboard::ReadImage(Buffer buffer) const {
  ScopedGObject<GdkPixbuf>::Type pixbuf(
      gtk_clipboard_wait_for_image(clipboard_));
  if (!pixbuf.get())
    return SkBitmap();

  gfx::CanvasSkia canvas(gdk_pixbuf_get_width(pixbuf.get()),
                         gdk_pixbuf_get_height(pixbuf.get()),
                         false);
  {
    skia::ScopedPlatformPaint scoped_platform_paint(canvas.sk_canvas());
    cairo_t* context = scoped_platform_paint.GetPlatformSurface();
    gdk_cairo_set_source_pixbuf(context, pixbuf.get(), 0.0, 0.0);
    cairo_paint(context);
  }
  return canvas.ExtractBitmap();
}
