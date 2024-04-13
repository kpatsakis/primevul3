void RootWindow::OnWindowRemovedFromRootWindow(Window* detached) {
  DCHECK(capture_window_ != this);

  OnWindowHidden(detached, false);

  if (detached->IsVisible() &&
      detached->ContainsPointInRoot(last_mouse_location_)) {
    PostMouseMoveEventAfterWindowChange();
  }
}
