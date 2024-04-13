bool SerializedFlashMenu::SetPPMenu(const PP_Flash_Menu* menu) {
  DCHECK(!pp_menu_);
  if (!CheckMenu(0, menu))
    return false;
  pp_menu_ = menu;
  own_menu_ = false;
  return true;
}
