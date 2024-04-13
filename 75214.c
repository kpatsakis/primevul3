gboolean BrowserTitlebar::OnWindowStateChanged(GtkWindow* window,
    GdkEventWindowState* event, BrowserTitlebar* titlebar) {
  if (titlebar->browser_window_->IsMaximized()) {
    gtk_widget_hide(titlebar->maximize_button_->widget());
    gtk_widget_show(titlebar->restore_button_->widget());
  } else {
    gtk_widget_hide(titlebar->restore_button_->widget());
    gtk_widget_show(titlebar->maximize_button_->widget());
  }
  titlebar->UpdateTitlebarAlignment();
  titlebar->UpdateTextColor();
  return FALSE;
}
