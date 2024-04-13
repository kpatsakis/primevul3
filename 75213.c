gboolean OnMouseMoveEvent(GtkWidget* widget, GdkEventMotion* event,
                          BrowserWindowGtk* browser_window) {
  browser_window->ResetCustomFrameCursor();
  return TRUE;
}
