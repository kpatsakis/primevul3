void MenuGtk::UpdateMenu() {
  gtk_container_foreach(GTK_CONTAINER(menu_.get()), SetMenuItemInfo, this);
}
