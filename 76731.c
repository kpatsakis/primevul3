bool RootWindow::DispatchCancelTouchEvent(ui::TouchEvent* event) {
  return DispatchTouchEvent(static_cast<TouchEvent*>(event));
}
