void MenuGtk::SetMenuItemInfo(GtkWidget* widget, gpointer userdata) {
  if (GTK_IS_SEPARATOR_MENU_ITEM(widget)) {
    return;
  }

  MenuGtk* menu = reinterpret_cast<MenuGtk*>(userdata);
  int id;
  const MenuCreateMaterial* data =
      reinterpret_cast<const MenuCreateMaterial*>(
          g_object_get_data(G_OBJECT(widget), "menu-data"));
  if (data) {
    id = data->id;
  } else {
    id = reinterpret_cast<intptr_t>(g_object_get_data(G_OBJECT(widget),
                                    "menu-id"));
  }

  if (GTK_IS_CHECK_MENU_ITEM(widget)) {
    GtkCheckMenuItem* item = GTK_CHECK_MENU_ITEM(widget);

    block_activation_ = true;
    gtk_check_menu_item_set_active(item, menu->delegate_->IsItemChecked(id));
    block_activation_ = false;
  }

  if (GTK_IS_MENU_ITEM(widget)) {
    gtk_widget_set_sensitive(
        widget, menu->delegate_->IsCommandEnabled(id));

    GtkWidget* submenu = gtk_menu_item_get_submenu(GTK_MENU_ITEM(widget));
    if (submenu) {
      gtk_container_foreach(GTK_CONTAINER(submenu), &SetMenuItemInfo,
                            userdata);
    }
  }
}
