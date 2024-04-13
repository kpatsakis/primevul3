void RootWindow::HandleMouseCaptureChanged(Window* old_capture_window) {
  if (capture_window_)
    host_->SetCapture();
  else
    host_->ReleaseCapture();

  if (old_capture_window && old_capture_window->delegate()) {
    MouseEvent event(
        ui::ET_MOUSE_CAPTURE_CHANGED, gfx::Point(), gfx::Point(), 0);
    ProcessMouseEvent(old_capture_window, &event);

    old_capture_window->delegate()->OnCaptureLost();
  }
}
