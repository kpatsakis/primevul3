skia::PlatformCanvas* ImageDataPlatformBackend::GetPlatformCanvas() {
  return mapped_canvas_.get();
}
