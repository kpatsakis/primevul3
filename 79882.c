bool SerializedFlashMenu::ReadFromMessage(const IPC::Message* m,
                                          PickleIterator* iter) {
  DCHECK(!pp_menu_);
  pp_menu_ = ReadMenu(0, m, iter);
  if (!pp_menu_)
    return false;

  own_menu_ = true;
  return true;
}
