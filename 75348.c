bool OutdatedPluginInfoBarDelegate::Accept() {
  UserMetrics::RecordAction(UserMetricsAction("OutdatedPluginInfobar.Update"));
  tab_contents_->OpenURL(update_url_, GURL(), NEW_FOREGROUND_TAB,
                         PageTransition::LINK);
  return false;
}
