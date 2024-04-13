InterstitialPage::~InterstitialPage() {
  InterstitialPageMap::iterator iter = tab_to_interstitial_page_->find(tab_);
  DCHECK(iter != tab_to_interstitial_page_->end()) <<
      "InterstitialPage missing from map. Please add a comment to the bug "
      "http://crbug.com/9442 with the URL you were visiting";
  if (iter != tab_to_interstitial_page_->end())
    tab_to_interstitial_page_->erase(iter);
  DCHECK(!render_view_host_);
}
