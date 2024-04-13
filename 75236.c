void MenuGtk::PopupAsContext(guint32 event_time) {
  gtk_menu_popup(GTK_MENU(menu_.get()), NULL, NULL, NULL, NULL, 3, event_time);
}
