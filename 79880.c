void FreeMenu(const PP_Flash_Menu* menu) {
  if (menu->items) {
    for (uint32_t i = 0; i < menu->count; ++i)
      FreeMenuItem(menu->items + i);
    delete [] menu->items;
  }
  delete menu;
}
