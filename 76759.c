void RootWindow::OnWindowHidden(Window* invisible, bool destroyed) {
  if (invisible->Contains(focused_window_)) {
    Window* focus_to = invisible->transient_parent();
    if (focus_to) {
      if (destroyed)
        focus_to->RemoveTransientChild(invisible);
    } else {
      focus_to = invisible->parent();
    }
    if (focus_to &&
        (!focus_to->IsVisible() ||
         (client::GetActivationClient(this) &&
          !client::GetActivationClient(this)->OnWillFocusWindow(focus_to,
                                                                NULL)))) {
      focus_to = NULL;
    }
    SetFocusedWindow(focus_to, NULL);
  }
  if (invisible->Contains(capture_window_) && invisible != this)
    ReleaseCapture(capture_window_);

  if (invisible->Contains(mouse_pressed_handler_))
    mouse_pressed_handler_ = NULL;
  if (invisible->Contains(mouse_moved_handler_))
    mouse_moved_handler_ = NULL;
  gesture_recognizer_->FlushTouchQueue(invisible);
}
