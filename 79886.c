void WriteMenu(IPC::Message* m, const PP_Flash_Menu* menu) {
  m->WriteUInt32(menu->count);
  for (uint32_t i = 0; i < menu->count; ++i)
    WriteMenuItem(m, menu->items + i);
}
