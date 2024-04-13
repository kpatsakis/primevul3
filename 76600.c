void* ImageDataPlatformBackend::Map() {
  if (!mapped_canvas_.get()) {
    mapped_canvas_.reset(platform_image_->Map());
    if (!mapped_canvas_.get())
      return NULL;
  }
  const SkBitmap& bitmap =
      skia::GetTopDevice(*mapped_canvas_)->accessBitmap(true);

  const_cast<SkBitmap&>(bitmap).setIsOpaque(false);

  bitmap.lockPixels();
  return bitmap.getAddr32(0, 0);
}
