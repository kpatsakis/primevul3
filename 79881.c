void FreeMenuItem(const PP_Flash_MenuItem* menu_item) {
  if (menu_item->name)
    delete [] menu_item->name;
  if (menu_item->submenu)
    FreeMenu(menu_item->submenu);
}
