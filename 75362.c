bool OutdatedPluginInfoBarDelegate::LinkClicked(
    WindowOpenDisposition disposition) {
  UserMetrics::RecordAction(
      UserMetricsAction("OutdatedPluginInfobar.LearnMore"));
  return PluginInfoBarDelegate::LinkClicked(disposition);
}
