void InterstitialPage::InterstitialPageRVHViewDelegate::HandleKeyboardEvent(
    const NativeWebKeyboardEvent& event) {
  if (interstitial_page_->tab() && interstitial_page_->tab()->GetViewDelegate())
    interstitial_page_->tab()->GetViewDelegate()->HandleKeyboardEvent(event);
}
