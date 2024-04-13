void MenuGtk::Popup(GtkWidget* widget, gint button_type, guint32 timestamp) {
  gtk_menu_popup(GTK_MENU(menu_.get()), NULL, NULL,
                 MenuPositionFunc,
                 widget,
                 button_type, timestamp);
}
