void MenuGtk::AppendMenuItem(int command_id, GtkWidget* menu_item) {
  g_object_set_data(G_OBJECT(menu_item), "menu-id",
                    reinterpret_cast<void*>(command_id));

  g_signal_connect(G_OBJECT(menu_item), "activate",
                   G_CALLBACK(OnMenuItemActivated), this);

  gtk_widget_show(menu_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_.get()), menu_item);
}
