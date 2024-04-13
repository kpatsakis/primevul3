bool ReadMenuItem(int depth,
                  const IPC::Message* m,
                  PickleIterator* iter,
                  PP_Flash_MenuItem* menu_item) {
  uint32_t type;
  if (!m->ReadUInt32(iter, &type))
    return false;
  if (type > PP_FLASH_MENUITEM_TYPE_SUBMENU)
    return false;
  menu_item->type = static_cast<PP_Flash_MenuItem_Type>(type);
  std::string name;
  if (!m->ReadString(iter, &name))
    return false;
  menu_item->name = new char[name.size() + 1];
  std::copy(name.begin(), name.end(), menu_item->name);
  menu_item->name[name.size()] = 0;
  if (!m->ReadInt(iter, &menu_item->id))
    return false;
  if (!IPC::ParamTraits<PP_Bool>::Read(m, iter, &menu_item->enabled))
    return false;
  if (!IPC::ParamTraits<PP_Bool>::Read(m, iter, &menu_item->checked))
    return false;
  if (type == PP_FLASH_MENUITEM_TYPE_SUBMENU) {
    menu_item->submenu = ReadMenu(depth, m, iter);
    if (!menu_item->submenu)
      return false;
  }
  return true;
}
