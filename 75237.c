void MenuGtk::PopupAsFromKeyEvent(GtkWidget* widget) {
  Popup(widget, 0, gtk_get_current_event_time());
  gtk_menu_shell_select_first(GTK_MENU_SHELL(menu_.get()), FALSE);
}
