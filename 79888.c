SerializedFlashMenu::~SerializedFlashMenu() {
  if (own_menu_)
    FreeMenu(pp_menu_);
}
