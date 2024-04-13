NetworkScreen::NetworkScreen(WizardScreenDelegate* delegate)
    : ViewScreen<NetworkSelectionView>(delegate,
                                       kWelcomeScreenWidth,
                                       kWelcomeScreenHeight),
      is_network_subscribed_(false),
      continue_pressed_(false),
      bubble_(NULL) {
  language_switch_menu_.set_menu_alignment(views::Menu2::ALIGN_TOPLEFT);
}
