bool ImageDataPlatformBackend::IsMapped() const {
  return !!mapped_canvas_.get();
}
