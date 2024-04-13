void MenuGtk::AppendSeparator() {
  GtkWidget* menu_item = gtk_separator_menu_item_new();
  gtk_widget_show(menu_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_.get()), menu_item);
}
