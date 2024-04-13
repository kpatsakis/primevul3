bool BlockedPluginInfoBarDelegate::LinkClicked(
    WindowOpenDisposition disposition) {
  UserMetrics::RecordAction(
      UserMetricsAction("BlockedPluginInfobar.LearnMore"));
  return PluginInfoBarDelegate::LinkClicked(disposition);
}
