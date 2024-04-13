bool RootWindow::DispatchLongPressGestureEvent(ui::GestureEvent* event) {
  return DispatchGestureEvent(static_cast<GestureEvent*>(event));
}
