void RootWindowHostWin::ReleaseCapture() {
  if (has_capture_) {
    has_capture_ = false;
    ::ReleaseCapture();
  }
}
