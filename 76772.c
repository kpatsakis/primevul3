void RootWindow::SetCapture(Window* window) {
  if (capture_window_ == window)
    return;

  gesture_recognizer_->CancelNonCapturedTouches(window);

  aura::Window* old_capture_window = capture_window_;
  capture_window_ = window;

  HandleMouseCaptureChanged(old_capture_window);

  if (capture_window_) {
    if (mouse_moved_handler_ || mouse_button_flags_ != 0)
      mouse_moved_handler_ = capture_window_;
  } else {
    mouse_moved_handler_ = NULL;
  }
  mouse_pressed_handler_ = NULL;
}
