void WebContentsImpl::DetachInterstitialPage() {
  if (GetInterstitialPage())
    render_manager_.remove_interstitial_page();
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidDetachInterstitialPage());
}
