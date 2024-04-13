InterstitialPage* InterstitialPage::GetInterstitialPage(
    TabContents* tab_contents) {
  InitInterstitialPageMap();
  InterstitialPageMap::const_iterator iter =
      tab_to_interstitial_page_->find(tab_contents);
  if (iter == tab_to_interstitial_page_->end())
    return NULL;

  return iter->second;
}
