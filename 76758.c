void RootWindow::OnWindowDestroying(Window* window) {
  OnWindowHidden(window, true);

  if (window->IsVisible() &&
      window->ContainsPointInRoot(last_mouse_location_)) {
    PostMouseMoveEventAfterWindowChange();
  }
}
