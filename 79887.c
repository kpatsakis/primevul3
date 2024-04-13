void SerializedFlashMenu::WriteToMessage(IPC::Message* m) const {
  WriteMenu(m, pp_menu_);
}
