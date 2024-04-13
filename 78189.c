bool WebContentsImpl::ShowingInterstitialPage() const {
  return render_manager_.interstitial_page() != NULL;
}
