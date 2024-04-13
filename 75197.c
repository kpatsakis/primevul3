void BrowserTitlebar::ActiveWindowChanged(GdkWindow* active_window) {
  if (!window_)
    return;

  window_has_focus_ = GTK_WIDGET(window_)->window == active_window;
  UpdateTextColor();
}
