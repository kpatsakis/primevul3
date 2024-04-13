void MenuGtk::MenuPositionFunc(GtkMenu* menu,
                               int* x,
                               int* y,
                               gboolean* push_in,
                               void* void_widget) {
  GtkWidget* widget = GTK_WIDGET(void_widget);
  GtkRequisition menu_req;

  gtk_widget_size_request(GTK_WIDGET(menu), &menu_req);

  gdk_window_get_origin(widget->window, x, y);
  GdkScreen *screen = gtk_widget_get_screen(widget);
  gint monitor = gdk_screen_get_monitor_at_point(screen, *x, *y);

  GdkRectangle screen_rect;
  gdk_screen_get_monitor_geometry(screen, monitor,
                                  &screen_rect);

  if (GTK_WIDGET_NO_WINDOW(widget)) {
    *x += widget->allocation.x;
    *y += widget->allocation.y;
  }
  *y += widget->allocation.height;

  bool start_align =
    !!g_object_get_data(G_OBJECT(widget), "left-align-popup");
  if (l10n_util::GetTextDirection() == l10n_util::RIGHT_TO_LEFT)
    start_align = !start_align;

  if (!start_align)
    *x += widget->allocation.width - menu_req.width;

  if (*y + menu_req.height >= screen_rect.height &&
      *y > screen_rect.height / 2) {
    *y -= menu_req.height;
  }

  *push_in = FALSE;
}
