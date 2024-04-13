GtkWidget* MenuGtk::BuildMenuItemWithImage(const std::string& label,
                                           const SkBitmap& icon) {
  std::string converted_label = ConvertAcceleratorsFromWindowsStyle(label);
  GtkWidget* menu_item =
      gtk_image_menu_item_new_with_mnemonic(converted_label.c_str());

  GdkPixbuf* pixbuf = gfx::GdkPixbufFromSkBitmap(&icon);
  gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_item),
                                gtk_image_new_from_pixbuf(pixbuf));
  g_object_unref(pixbuf);
  if (delegate_->AlwaysShowImages())
    gtk_util::SetAlwaysShowImage(menu_item);

  return menu_item;
}
