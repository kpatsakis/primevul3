void WebContentsImpl::FocusThroughTabTraversal(bool reverse) {
  if (ShowingInterstitialPage()) {
    render_manager_.interstitial_page()->FocusThroughTabTraversal(reverse);
    return;
  }
  GetRenderViewHostImpl()->SetInitialFocus(reverse);
}
