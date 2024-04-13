void BlockedPluginInfoBarDelegate::InfoBarDismissed() {
  UserMetrics::RecordAction(
      UserMetricsAction("BlockedPluginInfobar.Dismissed"));
}
