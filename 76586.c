const SkBitmap* ImageDataNaClBackend::GetMappedBitmap() const {
  if (!IsMapped())
    return NULL;
  return &skia_bitmap_;
}
