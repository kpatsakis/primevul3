SkCanvas* ImageDataPlatformBackend::GetCanvas() {
  return mapped_canvas_.get();
}
