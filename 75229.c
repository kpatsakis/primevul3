void MenuGtk::Cancel() {
  gtk_menu_popdown(GTK_MENU(menu_.get()));
}
