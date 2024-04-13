void MenuGtk::AppendCheckMenuItemWithLabel(int command_id,
                                           const std::string& label) {
  std::string converted_label = ConvertAcceleratorsFromWindowsStyle(label);
  GtkWidget* menu_item =
      gtk_check_menu_item_new_with_mnemonic(converted_label.c_str());
  AppendMenuItem(command_id, menu_item);
}
