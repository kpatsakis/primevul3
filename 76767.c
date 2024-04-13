void RootWindow::ReleaseCapture(Window* window) {
  if (capture_window_ != window)
    return;
  SetCapture(NULL);
}
