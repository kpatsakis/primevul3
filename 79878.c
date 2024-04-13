bool CheckMenu(int depth, const PP_Flash_Menu* menu) {
  if (depth > kMaxMenuDepth || !menu)
    return false;
  ++depth;

  if (menu->count && !menu->items)
    return false;

  for (uint32_t i = 0; i < menu->count; ++i) {
    if (!CheckMenuItem(depth, menu->items + i))
      return false;
  }
  return true;
}
