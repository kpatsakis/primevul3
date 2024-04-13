const SkBitmap* ImageDataPlatformBackend::GetMappedBitmap() const {
  if (!mapped_canvas_.get())
    return NULL;
  return &skia::GetTopDevice(*mapped_canvas_)->accessBitmap(false);
}
