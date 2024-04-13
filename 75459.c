gboolean BrowserFrameGtk::OnConfigureEvent(GtkWidget* widget,
                                           GdkEventConfigure* event) {
  browser_view_->WindowMoved();
  return views::NativeWidgetGtk::OnConfigureEvent(widget, event);
}
