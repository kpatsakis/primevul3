gboolean BrowserTitlebar::OnButtonPressed(GtkWidget* widget,
                                          GdkEventButton* event,
                                          BrowserTitlebar* titlebar) {
  if (event->button != 1)
    return FALSE;

  titlebar->ShowFaviconMenu(event);
  return TRUE;
}
