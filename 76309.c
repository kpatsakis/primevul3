void NetworkScreen::CreateView() {
  language_switch_menu_.InitLanguageMenu();
  ViewScreen<NetworkSelectionView>::CreateView();
}
