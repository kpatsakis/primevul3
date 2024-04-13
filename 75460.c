gboolean BrowserFrameGtk::OnWindowStateEvent(GtkWidget* widget,
                                             GdkEventWindowState* event) {
  bool was_full_screen = IsFullscreen();
  gboolean result = views::NativeWidgetGtk::OnWindowStateEvent(widget, event);
  if ((!IsVisible() || IsMinimized()) && browser_view_->GetStatusBubble()) {
    browser_view_->GetStatusBubble()->Hide();
  }
  if (was_full_screen != IsFullscreen())
    browser_view_->FullScreenStateChanged();
  return result;
}
