void RootWindow::OnWindowBoundsChanged(Window* window,
                                       bool contained_mouse_point) {
  if (contained_mouse_point ||
      (window->IsVisible() &&
       window->ContainsPointInRoot(last_mouse_location_))) {
    PostMouseMoveEventAfterWindowChange();
  }
}
