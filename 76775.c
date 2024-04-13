void RootWindow::SetFocusedWindow(Window* focused_window,
                                  const aura::Event* event) {
  if (focused_window == focused_window_)
    return;
  if (focused_window && !focused_window->CanFocus())
    return;
  if (focused_window && client::GetActivationClient(this) &&
      !client::GetActivationClient(this)->OnWillFocusWindow(focused_window,
                                                            event)) {
    return;
  }

  Window* old_focused_window = focused_window_;
  focused_window_ = focused_window;
  if (old_focused_window && old_focused_window->delegate())
    old_focused_window->delegate()->OnBlur();
  if (focused_window_ && focused_window_->delegate())
    focused_window_->delegate()->OnFocus();
  if (focused_window_) {
    FOR_EACH_OBSERVER(RootWindowObserver, observers_,
                      OnWindowFocused(focused_window_));
  }
}
