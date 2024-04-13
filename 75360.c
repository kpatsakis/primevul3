bool PluginInfoBarDelegate::LinkClicked(WindowOpenDisposition disposition) {
  GURL url = google_util::AppendGoogleLocaleParam(GURL(GetLearnMoreURL()));
  tab_contents_->OpenURL(url, GURL(), NEW_FOREGROUND_TAB, PageTransition::LINK);
  return false;
}
