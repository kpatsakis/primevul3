void InterstitialPage::FocusThroughTabTraversal(bool reverse) {
  render_view_host_->SetInitialFocus(reverse);
}
