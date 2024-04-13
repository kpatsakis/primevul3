void RootWindowHostWin::SetCapture() {
  if (!has_capture_) {
    has_capture_ = true;
    ::SetCapture(hwnd());
  }
}
