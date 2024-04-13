bool InterstitialPage::InterstitialPageRVHViewDelegate::PreHandleKeyboardEvent(
    const NativeWebKeyboardEvent& event, bool* is_keyboard_shortcut) {
  if (interstitial_page_->tab() && interstitial_page_->tab()->GetViewDelegate())
    return interstitial_page_->tab()->GetViewDelegate()->PreHandleKeyboardEvent(
        event, is_keyboard_shortcut);
  return false;
}
